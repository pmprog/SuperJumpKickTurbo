
#include "framework.h"
#include "../Game/boot.h"

#ifdef PANDORA
#include <signal.h>
#endif

Framework* Framework::System;

Framework::Framework( int Width, int Height, int Framerate, bool DropFrames )
{
	System = this;

#ifdef WRITE_LOG
	LogFile = fopen( "sjkt.txt", "a" );
	
	fprintf( LogFile, "Framework: Startup: Allegro\n" );
#endif

	if( !al_init() )
	{
		fprintf( LogFile, "Framework: Error: Cannot init Allegro\n" );
		quitProgram = true;
		return;
	}
	
	al_init_font_addon();
	if( !al_install_keyboard() || !al_install_mouse() || !al_install_joystick() || !al_init_primitives_addon() || !al_init_ttf_addon() || !al_init_image_addon() )
	{
		fprintf( LogFile, "Framework: Error: Cannot init Allegro plugin\n" );
		quitProgram = true;
		return;
	}

#ifdef NETWORK_SUPPORT
#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Network\n" );
#endif
	if( enet_initialize() != 0 )
	{
		fprintf( LogFile, "Framework: Error: Cannot init enet\n" );
		quitProgram = true;
		return;
	}
#endif

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Default Variables\n" );
#endif
	quitProgram = false;
  ProgramStages = new StageStack();
  framesToProcess = 0;
	framesPerSecond = Framerate;
	enableSlowDown = DropFrames;

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Settings file\n" );
#endif
  Settings = new ConfigFile( "settings.cfg" );


#ifdef DOWNLOAD_SUPPORT
#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Download Manager\n" );
#endif
	DOWNLOADS = new DownloadManager( Settings->GetQuickIntegerValue( "Downloads.Concurrent", 3 ) );
#endif

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Allegro Events\n" );
#endif
	eventAllegro = al_create_event_queue();
	eventMutex = al_create_mutex_recursive();
	frameTimer = al_create_timer( 1.0 / (double)framesPerSecond );

	srand( (unsigned int)al_get_time() );

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Display\n" );
#endif
	DISPLAY = new Display( Width, Height );
	DISPLAY->Initialise( Settings->GetQuickIntegerValue( "Video.Width", Width ), Settings->GetQuickIntegerValue( "Video.Height", Height ), Settings->GetQuickBooleanValue( "Video.Fullscreen", false ), (DisplayScaleMode::ScaleMode)Settings->GetQuickIntegerValue( "Video.ScaleMode", DisplayScaleMode::Letterbox ) );
	AUDIO = new Audio();

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Register event sources\n" );
#endif
	RegisterEventSource( DISPLAY->GetEventSource() );
	RegisterEventSource( al_get_keyboard_event_source() );
	RegisterEventSource( al_get_mouse_event_source() );
	RegisterEventSource( al_get_joystick_event_source() );
	RegisterEventSource( al_get_timer_event_source( frameTimer ) );

#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Startup: Joystick IDs\n" );
#endif
	GetJoystickIDs();
}

Framework::~Framework()
{
#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Save Config\n" );
#endif
  SaveSettings();

#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Clear stages\n" );
#endif
	if( ProgramStages != 0 )
	{
		// Just make sure all stages are popped and deleted
		ShutdownFramework();
	}

#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown\n" );
#endif
	al_unregister_event_source( eventAllegro, DISPLAY->GetEventSource() );
	al_destroy_event_queue( eventAllegro );
	al_destroy_mutex( eventMutex );
	al_destroy_timer( frameTimer );

#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown display\n" );
#endif
	DISPLAY->Shutdown();
	delete DISPLAY;

#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown audio\n" );
#endif
	delete AUDIO;
	
#ifdef NETWORK_SUPPORT
#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown enet\n" );
#endif
	enet_deinitialize();
#endif

#ifdef DOWNLOAD_SUPPORT
#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown downloads\n" );
#endif
	DOWNLOADS->AbortDownloads = true;
	delete DOWNLOADS;
#endif

#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Shutdown Allegro\n" );
#endif
	al_uninstall_system();

#ifdef WRITE_LOG
	fclose( LogFile );
#endif

#ifdef PANDORA
	// Hacky close on the Pandora :(
	raise(SIGINT);
#endif
}

void Framework::Run()
{
#ifdef WRITE_LOG
  fprintf( LogFile, "Framework: Run.Program Loop\n" );
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
#ifdef WRITE_LOG
  // fprintf( LogFile, "Framework: ProcessEvents (%d events queued)\n", eventQueue.size() );
#endif

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
				GetJoystickIDs();
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
			case ALLEGRO_EVENT_JOYSTICK_AXIS:
				fwE = new Event();
				fwE->Type = EVENT_JOYSTICK_AXIS;
				fwE->Data.Joystick.ID = -1;
				for( int i = 0; i < al_get_num_joysticks(); i++ )
				{
					if( joystickIDs.at( i ) == e.joystick.id )
					{
						fwE->Data.Joystick.ID = i;
						break;
					}
				}
				fwE->Data.Joystick.Stick = e.joystick.stick;
				fwE->Data.Joystick.Axis = e.joystick.axis;
				fwE->Data.Joystick.Position = e.joystick.pos;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
				fwE = new Event();
				fwE->Type = EVENT_JOYSTICK_BUTTON_DOWN;
				fwE->Data.Joystick.ID = -1;
				for( int i = 0; i < al_get_num_joysticks(); i++ )
				{
					if( joystickIDs.at( i ) == e.joystick.id )
					{
						fwE->Data.Joystick.ID = i;
						break;
					}
				}
				fwE->Data.Joystick.Button = e.joystick.button;
				PushEvent( fwE );
				break;
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
				fwE = new Event();
				fwE->Type = EVENT_JOYSTICK_BUTTON_UP;
				fwE->Data.Joystick.ID = -1;
				for( int i = 0; i < al_get_num_joysticks(); i++ )
				{
					if( joystickIDs.at( i ) == e.joystick.id )
					{
						fwE->Data.Joystick.ID = i;
						break;
					}
				}
				fwE->Data.Joystick.Button = e.joystick.button;
				PushEvent( fwE );
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
  fprintf( LogFile, "Framework: Shutdown Framework\n" );
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

void Framework::GetJoystickIDs()
{
#ifdef WRITE_LOG
	fprintf( LogFile, "Framework: Joysticks: Learn Joystick IDs\n" );
#endif
	// Record joystick IDs in a list for ID conversion
	joystickIDs.clear();
	for( int i = 0; i < al_get_num_joysticks(); i++ )
	{
		joystickIDs.push_back( al_get_joystick( i ) );
	}
}