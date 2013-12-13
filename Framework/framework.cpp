#include "framework.h"
#include "../Game/boot.h"

Framework* Framework::System;

Framework::Framework()
{
#ifdef WRITE_LOG
  printf( "Framework: Startup\n" );
#endif
	spInitCore();
	spInitPrimitives();
	SDL_EnableUNICODE( 1 );

	quitProgram = false;
  ProgramStages = new StageStack();
	deltaTime = 0;
  framesToProcess = 0;
  Settings = new ConfigFile( "settings.cfg" );

  InitialiseDisplay();
	InitialiseAudioSystem();

	eventMutex = SDL_CreateMutex();

  System = this;
}

Framework::~Framework()
{
#ifdef WRITE_LOG
  printf( "Framework: Save Config\n" );
#endif
  SaveSettings();

#ifdef WRITE_LOG
  printf( "Framework: Shutdown\n" );
#endif
	SDL_DestroyMutex( eventMutex );
	ShutdownDisplay();
	ShutdownAudioSystem();

	//spQuitPrimitives();
	spQuitCore();
}

void Framework::Run()
{
#ifdef WRITE_LOG
  printf( "Framework: Run.Program Loop\n" );
#endif

  // Set BootUp Stage active
  ProgramStages->Push( (Stage*)new BootUp() );

	spLoop( engineDraw, engineUpdate, FRAME_TIME_IN_MS, resizeWindow, pushSDLEvent);
}

void Framework::ProcessEvents()
{
#ifdef WRITE_LOG
  printf( "Framework: ProcessEvents\n" );
#endif

	if( ProgramStages->IsEmpty() )
  {
    quitProgram = true;
    return;
	}

	SDL_LockMutex( eventMutex );

	while( eventQueue.size() > 0 && !ProgramStages->IsEmpty() )
	{
		Event* e;
		e = eventQueue.front();
		eventQueue.pop_front();
		switch( e->Type )
		{
			case EVENT_WINDOW_CLOSED:
				SDL_UnlockMutex( eventMutex );
				ShutdownFramework();
				return;
				break;
			case EVENT_WINDOW_RESIZE:
				resizeWindow( e->Data.Display.Resize.w, e->Data.Display.Resize.h );
				break;
			default:
				ProgramStages->Current()->EventOccurred( e );
				break;
		}
		delete e;
	}

	SDL_UnlockMutex( eventMutex );
}

void Framework::ProcessUpdates( int Delta )
{
	deltaTime += Delta;

	while( deltaTime >= FRAME_TIME_IN_MS && !ProgramStages->IsEmpty() )
	{
		ProgramStages->Current()->Update();
		deltaTime -= FRAME_TIME_IN_MS;
	}
}

void Framework::PushEvent( Event* e )
{
	SDL_LockMutex( eventMutex );
	eventQueue.push_back( e );
	SDL_UnlockMutex( eventMutex );
}

void Framework::PushSDLEvent( SDL_Event* e )
{
	SDL_LockMutex( eventMutex );
	eventQueue.push_back( new Event( e ) );
	SDL_UnlockMutex( eventMutex );
}

void Framework::ShutdownFramework()
{
  while( !ProgramStages->IsEmpty() )
  {
    delete ProgramStages->Pop();
  }
  quitProgram = true;
}

int Framework::GetDisplayWidth()
{
  return displaySurface->w;
}

int Framework::GetDisplayHeight()
{
	return displaySurface->h;
}

void Framework::PlayMusic( std::string Filename, bool Loop )
{
#ifdef WRITE_LOG
  printf( "Framework: Play Music '%s'\n", Filename.c_str() );
#endif

}

void Framework::StopMusic()
{
#ifdef WRITE_LOG
  printf( "Framework: Stop Music\n" );
#endif
}

void Framework::SaveSettings()
{
  // Just to keep the filename consistant
  Settings->Save( "settings.cfg" );
}

void Framework::SetWindowTitle( std::string* NewTitle )
{
  SDL_WM_SetCaption( NewTitle->c_str(), 0 );
}

void Framework::InitialiseAudioSystem()
{
#ifdef WRITE_LOG
  printf( "Framework: Initialise Audio\n" );
#endif
	spSoundInit();
}

void Framework::ShutdownAudioSystem()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Audio\n" );
#endif
	spSoundQuit();
}

void Framework::InitialiseDisplay()
{
#ifdef WRITE_LOG
  printf( "Framework: Initialise Display\n" );
#endif

#ifdef X86CPU
	int scrW = 800;
	int scrH = 600;
	bool scrFS = false;
#else
	int scrW = 800;
	int scrH = 480;
	bool scrFS = true;
#endif

	if( Settings->KeyExists( "Visual.ScreenWidth" ) )
  {
    Settings->GetIntegerValue( "Visual.ScreenWidth", &scrW );
	} else {
		Settings->SetIntegerValue( "Visual.ScreenWidth", scrW );
	}
	if( Settings->KeyExists( "Visual.ScreenHeight" ) )
  {
    Settings->GetIntegerValue( "Visual.ScreenHeight", &scrH );
	} else {
		Settings->SetIntegerValue( "Visual.ScreenHeight", scrH );
	}
	if( Settings->KeyExists( "Visual.FullScreen" ) )
  {
    Settings->GetBooleanValue( "Visual.FullScreen", &scrFS );
	} else {
		Settings->SetBooleanValue( "Visual.FullScreen", scrFS );
	}

	displaySurface = spCreateWindow( scrW, scrH, scrFS, 0 );
	spSelectRenderTarget( spGetWindowSurface() );
	spSetPerspective( 50.0f, (float)spGetWindowSurface()->w / (float)spGetWindowSurface()->h, 0.1f, 100.0f);

	spSetZTest( 0 );
	spSetZSet( 0 );
	spSetAlphaTest( 1 );
}

void Framework::ShutdownDisplay()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Display\n" );
#endif
	spDeleteSurface(displaySurface);
}


int engineUpdate(Uint32 steps)
{
	if( Framework::System->IsShuttingDown() || Framework::System->ProgramStages->IsEmpty() )
	{
		return 1;
	}
	Framework::System->ProcessEvents();
	Framework::System->ProcessUpdates( steps );
	return 0;
}

void engineDraw()
{
#ifdef WRITE_LOG
  printf( "Framework: engineDraw\n" );
#endif
  if( !Framework::System->ProgramStages->IsEmpty() )
  {
		spResetZBuffer();
    Framework::System->ProgramStages->Current()->Render();
		spFlip();
  }
}

void resizeWindow( Uint16 w, Uint16 h )
{
	Framework::System->ShutdownDisplay();

  Framework::System->Settings->SetIntegerValue( "Visual.ScreenWidth", w );
  Framework::System->Settings->SetIntegerValue( "Visual.ScreenHeight", h );

	Framework::System->InitialiseDisplay();
}

void pushSDLEvent( SDL_Event* e )
{
	Framework::System->PushSDLEvent( e );
}