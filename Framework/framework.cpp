
#include "framework.h"
#include "../Game/boot.h"

Framework* Framework::System;

Framework::Framework( int Width, int Height, int Framerate, bool DropFrames )
{
#ifdef WRITE_LOG
	printf( "Framework: Startup: Allegro\n" );
#endif

	if( !al_init() )
	{
		printf( "Framework: Error: Cannot init Allegro\n" );
		quitProgram = true;
		return;
	}
	
	al_init_font_addon();
	if( !al_install_keyboard() || !al_install_mouse() || !al_init_primitives_addon() || !al_init_ttf_addon() || !al_init_image_addon() )
	{
		printf( "Framework: Error: Cannot init Allegro plugin\n" );
		quitProgram = true;
		return;
	}

#ifdef NETWORK_SUPPORT
#ifdef WRITE_LOG
	printf( "Framework: Startup: Network\n" );
#endif
	if( enet_initialize() != 0 )
	{
		printf( "Framework: Error: Cannot init enet\n" );
		quitProgram = true;
		return;
	}
#endif

#ifdef WRITE_LOG
	printf( "Framework: Startup: Default Variables\n" );
#endif
	quitProgram = false;
  ProgramStages = new StageStack();
  framesToProcess = 0;
	framesPerSecond = Framerate;
	enableSlowDown = DropFrames;

#ifdef WRITE_LOG
	printf( "Framework: Startup: Settings file\n" );
#endif
  Settings = new ConfigFile( "settings.cfg" );


#ifdef DOWNLOAD_SUPPORT
#ifdef WRITE_LOG
	printf( "Framework: Startup: Download Manager\n" );
#endif
	DOWNLOADS = new DownloadManager( Settings->GetQuickIntegerValue( "Downloads.Concurrent", 3 ) );
#endif

#ifdef WRITE_LOG
	printf( "Framework: Startup: Allegro Events\n" );
#endif
	eventAllegro = al_create_event_queue();
	eventMutex = al_create_mutex_recursive();
	frameTimer = al_create_timer( 1.0 / (double)framesPerSecond );

	srand( (unsigned int)al_get_time() );

#ifdef WRITE_LOG
	printf( "Framework: Startup: Display\n" );
#endif
	DISPLAY = new Display( Width, Height );
	DISPLAY->Initialise( Settings->GetQuickIntegerValue( "Video.Width", Width ), Settings->GetQuickIntegerValue( "Video.Height", Height ), Settings->GetQuickBooleanValue( "Video.Fullscreen", false ), (DisplayScaleMode::ScaleMode)Settings->GetQuickIntegerValue( "Video.ScaleMode", DisplayScaleMode::Letterbox ) );
	AUDIO = new Audio();

#ifdef WRITE_LOG
	printf( "Framework: Startup: Register event sources\n" );
#endif
	RegisterEventSource( DISPLAY->GetEventSource() );
	RegisterEventSource( al_get_keyboard_event_source() );
	RegisterEventSource( al_get_mouse_event_source() );
	RegisterEventSource( al_get_timer_event_source( frameTimer ) );

	System = this;
}

Framework::~Framework()
{
#ifdef WRITE_LOG
  printf( "Framework: Save Config\n" );
#endif
  SaveSettings();

#ifdef WRITE_LOG
  printf( "Framework: Clear stages\n" );
#endif
	if( ProgramStages != 0 )
	{
		// Just make sure all stages are popped and deleted
		ShutdownFramework();
	}

#ifdef WRITE_LOG
  printf( "Framework: Shutdown\n" );
#endif
	al_unregister_event_source( eventAllegro, DISPLAY->GetEventSource() );
	al_destroy_event_queue( eventAllegro );
	al_destroy_mutex( eventMutex );
	al_destroy_timer( frameTimer );

#ifdef WRITE_LOG
  printf( "Framework: Shutdown display\n" );
#endif
	DISPLAY->Shutdown();
	delete DISPLAY;

#ifdef WRITE_LOG
  printf( "Framework: Shutdown audio\n" );
#endif
	delete AUDIO;
	
#ifdef NETWORK_SUPPORT
#ifdef WRITE_LOG
  printf( "Framework: Shutdown enet\n" );
#endif
	enet_deinitialize();
#endif

#ifdef DOWNLOAD_SUPPORT
#ifdef WRITE_LOG
  printf( "Framework: Shutdown downloads\n" );
#endif
	DOWNLOADS->AbortDownloads = true;
	delete DOWNLOADS;
#endif

#ifdef WRITE_LOG
  printf( "Framework: Shutdown Allegro\n" );
#endif
	al_uninstall_system();
}

void Framework::Run()
{
#ifdef WRITE_LOG
  printf( "Framework: Run.Program Loop\n" );
#endif

  ProgramStages->Push( new BootUp() );

	al_start_timer( frameTimer );

	while( !quitProgram )
	{
		ProcessEvents();
		while( framesToProcess > 0 )
		{
			if( ProgramStages->IsEmpty() )
			{
				break;
			}
			ProgramStages->Current()->Update();
#ifdef DOWNLOAD_SUPPORT
			DOWNLOADS->Update();
#endif
			framesToProcess--;
		}
		if( !ProgramStages->IsEmpty() )
		{
			ProgramStages->Current()->Render();
			DISPLAY->Render();
		}
	}
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

	// Convert Allegro events before we process
	// TODO: Consider threading the translation
	TranslateAllegroEvents();

	al_lock_mutex( eventMutex );

	while( eventQueue.size() > 0 && !ProgramStages->IsEmpty() )
	{
		Event* e;
		e = eventQueue.front();
		eventQueue.pop_front();
		switch( e->Type )
		{
			case EVENT_WINDOW_CLOSED:
				delete e;
				al_unlock_mutex( eventMutex );
				ShutdownFramework();
				return;
				break;
			default:
				ProgramStages->Current()->EventOccurred( e );
#ifdef DOWNLOAD_SUPPORT
				DOWNLOADS->EventOccured( e );
#endif
				break;
		}
		delete e;
	}

	al_unlock_mutex( eventMutex );
}

void Framework::PushEvent( Event* e )
{
	al_lock_mutex( eventMutex );
	eventQueue.push_back( e );
	al_unlock_mutex( eventMutex );
}

void Framework::TranslateAllegroEvents()
{
	ALLEGRO_EVENT e;
	Event* fwE;

	while( al_get_next_event( eventAllegro, &e ) )
	{
		switch( e.type )
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				fwE = new Event();
				fwE->Type = EVENT_WINDOW_CLOSED;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
				al_reconfigure_joysticks();
				break;
			case ALLEGRO_EVENT_TIMER:
				if( e.timer.source == frameTimer )
				{
					if( enableSlowDown )
					{
						// Slow the game down, never process more than one update per frame
						framesToProcess = 1;
					} else {
						framesToProcess++;
					}
				} else {
					fwE = new Event();
					fwE->Type = EVENT_TIMER_TICK;
					fwE->Data.Timer.TimerObject = (void*)e.timer.source;
					PushEvent( fwE );
				}
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				fwE = new Event();
				fwE->Type = EVENT_KEY_DOWN;
				fwE->Data.Keyboard.KeyCode = e.keyboard.keycode;
				fwE->Data.Keyboard.UniChar = e.keyboard.unichar;
				fwE->Data.Keyboard.Modifiers = e.keyboard.modifiers;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_KEY_UP:
				fwE = new Event();
				fwE->Type = EVENT_KEY_UP;
				fwE->Data.Keyboard.KeyCode = e.keyboard.keycode;
				fwE->Data.Keyboard.UniChar = e.keyboard.unichar;
				fwE->Data.Keyboard.Modifiers = e.keyboard.modifiers;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				fwE = new Event();
				fwE->Type = EVENT_KEY_PRESS;
				fwE->Data.Keyboard.KeyCode = e.keyboard.keycode;
				fwE->Data.Keyboard.UniChar = e.keyboard.unichar;
				fwE->Data.Keyboard.Modifiers = e.keyboard.modifiers;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				fwE = new Event();
				fwE->Type = EVENT_MOUSE_MOVE;
				fwE->Data.Mouse.X = DISPLAY->ScreenXToGameX(e.mouse.x);
				fwE->Data.Mouse.Y = DISPLAY->ScreenYToGameY(e.mouse.y);
				fwE->Data.Mouse.DeltaX = DISPLAY->ScreenXToGameX(e.mouse.dx);
				fwE->Data.Mouse.DeltaY = DISPLAY->ScreenYToGameY(e.mouse.dy);
				fwE->Data.Mouse.WheelVertical = e.mouse.dz;
				fwE->Data.Mouse.WheelHorizontal = e.mouse.dw;
				fwE->Data.Mouse.Button = e.mouse.button;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				fwE = new Event();
				fwE->Type = EVENT_MOUSE_DOWN;
				fwE->Data.Mouse.X = DISPLAY->ScreenXToGameX(e.mouse.x);
				fwE->Data.Mouse.Y = DISPLAY->ScreenYToGameY(e.mouse.y);
				fwE->Data.Mouse.DeltaX = DISPLAY->ScreenXToGameX(e.mouse.dx);
				fwE->Data.Mouse.DeltaY = DISPLAY->ScreenYToGameY(e.mouse.dy);
				fwE->Data.Mouse.WheelVertical = e.mouse.dz;
				fwE->Data.Mouse.WheelHorizontal = e.mouse.dw;
				fwE->Data.Mouse.Button = e.mouse.button;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				fwE = new Event();
				fwE->Type = EVENT_MOUSE_UP;
				fwE->Data.Mouse.X = DISPLAY->ScreenXToGameX(e.mouse.x);
				fwE->Data.Mouse.Y = DISPLAY->ScreenYToGameY(e.mouse.y);
				fwE->Data.Mouse.DeltaX = DISPLAY->ScreenXToGameX(e.mouse.dx);
				fwE->Data.Mouse.DeltaY = DISPLAY->ScreenYToGameY(e.mouse.dy);
				fwE->Data.Mouse.WheelVertical = e.mouse.dz;
				fwE->Data.Mouse.WheelHorizontal = e.mouse.dw;
				fwE->Data.Mouse.Button = e.mouse.button;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				fwE = new Event();
				fwE->Type = EVENT_WINDOW_RESIZE;
				fwE->Data.Display.X = 0;
				fwE->Data.Display.Y = 0;
				fwE->Data.Display.Width = e.display.width;
				fwE->Data.Display.Height = e.display.height;
				fwE->Data.Display.Active = true;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
				fwE = new Event();
				fwE->Type = EVENT_WINDOW_ACTIVATE;
				fwE->Data.Display.X = 0;
				fwE->Data.Display.Y = 0;
				fwE->Data.Display.Width = e.display.width;
				fwE->Data.Display.Height = e.display.height;
				fwE->Data.Display.Active = true;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
				fwE = new Event();
				fwE->Type = EVENT_WINDOW_DEACTIVATE;
				fwE->Data.Display.X = 0;
				fwE->Data.Display.Y = 0;
				fwE->Data.Display.Width = e.display.width;
				fwE->Data.Display.Height = e.display.height;
				fwE->Data.Display.Active = false;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_AUDIO_STREAM_FINISHED:
				fwE = new Event();
				fwE->Type = EVENT_AUDIO_STREAM_FINISHED;
				PushEvent( fwE );
				break;
			default:
				fwE = new Event();
				fwE->Type = EVENT_UNDEFINED;
				PushEvent( fwE );
				break;
		}
	}
}

void Framework::ShutdownFramework()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Framework\n" );
#endif
  while( !ProgramStages->IsEmpty() )
  {
    delete ProgramStages->Pop();
  }
  quitProgram = true;
}

void Framework::SaveSettings()
{
  // Just to keep the filename consistant
  Settings->Save( "settings.cfg" );
}

bool Framework::IsSlowMode()
{
	return enableSlowDown;
}

void Framework::SetSlowMode(bool SlowEnabled)
{
	enableSlowDown = SlowEnabled;
}

int Framework::GetFramesPerSecond()
{
	return framesPerSecond;
}

void Framework::RegisterEventSource( ALLEGRO_EVENT_SOURCE* Source )
{
	if( Source != nullptr )
	{
		al_register_event_source( eventAllegro, Source );
	}
}

void Framework::UnregisterEventSource( ALLEGRO_EVENT_SOURCE* Source )
{
	if( Source != nullptr )
	{
		al_unregister_event_source( eventAllegro, Source );
	}
}
