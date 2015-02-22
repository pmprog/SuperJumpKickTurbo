
#include "display.h"
#include "../framework.h"

Display* Display::CurrentDisplay = nullptr;

Display::Display( int GameWidth, int GameHeight )
{
	gameSize.X = GameWidth;
	gameSize.Y = GameHeight;
	screenSize.X = GameWidth;
	screenSize.Y = GameHeight;
}

Display::~Display()
{
}

void Display::Initialise( int ScreenWidth, int ScreenHeight, bool Fullscreen, DisplayScaleMode::ScaleMode Scale )
{
#ifdef WRITE_LOG
  fprintf( FRAMEWORK->LogFile, "Framework: Initialise Display\n" );
#endif
	bool foundmode = false;
	bool foundwindowedmode = false;
	Vector2 fallback;

	screenSize.X = ScreenWidth;
	screenSize.Y = ScreenHeight;
	fallback.X = gameSize.X;
	fallback.Y = gameSize.Y;

#ifndef PANDORA

	if( Fullscreen )
	{
		al_set_new_display_flags( ALLEGRO_FULLSCREEN );
	}

	// Get Current Resolution
	for( int modeIdx = 0; modeIdx < al_get_num_display_modes(); modeIdx++ )
	{
		if( al_get_display_mode( modeIdx, &screenMode ) != NULL )
		{
			if( screenMode.width == ScreenWidth && screenMode.height == ScreenHeight )
			{
				foundmode = true;
			} else {
				if( !Fullscreen && screenMode.width > ScreenWidth && screenMode.height > ScreenHeight )
				{
					foundmode = true;	// We're windowed, and there's a resolution greater, so should be okay
				} else {
					fallback.X = screenMode.width;
					fallback.Y = screenMode.height;
				}
			}
		}
		if( foundmode )
		{
			break;
		}
	}

	if( foundmode )
	{
		screen = al_create_display( ScreenWidth, ScreenHeight );
	} else {
		screen = al_create_display( fallback.X, fallback.Y );
	}

	screenGameBuffer = al_create_bitmap( gameSize.X, gameSize.Y );
	al_set_target_bitmap( screenGameBuffer );

#else
	al_set_new_display_flags( ALLEGRO_FULLSCREEN );
	screen = al_create_display( 800, 480 );
#endif

	float bestscale = 1.0f;
	switch( Scale )
	{
		case DisplayScaleMode::Letterbox:
			bestscale = Maths::Min( screenSize.X / gameSize.X, screenSize.Y / gameSize.Y );
			gameScreenSize.X = gameSize.X * bestscale;
			gameScreenSize.Y = gameSize.Y * bestscale;
			gameScreenLocation.X = (screenSize.X / 2) - (gameScreenSize.X / 2);
			gameScreenLocation.Y = (screenSize.Y / 2) - (gameScreenSize.Y / 2);
			break;
		case DisplayScaleMode::Fill:
			gameScreenLocation.X = 0;
			gameScreenLocation.Y = 0;
			gameScreenSize.X = ScreenWidth;
			gameScreenSize.Y = ScreenHeight;
			break;
	}

	ClearTarget();

	al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );

	al_hide_mouse_cursor( screen );
}

void Display::Shutdown()
{
#ifdef WRITE_LOG
  fprintf( FRAMEWORK->LogFile, "Framework: Shutdown Display\n" );
#endif
	al_destroy_display( screen );
}

ALLEGRO_EVENT_SOURCE* Display::GetEventSource()
{
	return al_get_display_event_source( screen );
}

int Display::GetWidth()
{
	return gameSize.X;
}

int Display::GetHeight()
{
	return gameSize.Y;
}

void Display::SetTitle( std::string* NewTitle )
{
  al_set_app_name( NewTitle->c_str() );
	al_set_window_title( screen, NewTitle->c_str() );
}

void Display::SetTitle( std::string NewTitle )
{
  al_set_app_name( NewTitle.c_str() );
	al_set_window_title( screen, NewTitle.c_str() );
}

ALLEGRO_BITMAP* Display::GetCurrentTarget()
{
	if( screenRetarget != nullptr )
	{
		return screenRetarget;
	}
	return screenGameBuffer;
}

void Display::ClearTarget()
{
	screenRetarget = nullptr;

#ifdef PANDORA
	al_set_target_backbuffer( screen );
#else
	al_set_target_bitmap( screenGameBuffer );
#endif

}

void Display::SetTarget( ALLEGRO_BITMAP* Target )
{
	// If target is blank or back buffer, set properly
	if( Target == nullptr || al_get_backbuffer( screen ) == Target )
	{
		ClearTarget();
	} else {
		al_set_target_bitmap( Target );
		screenRetarget = Target;
	}
}

void Display::MouseVisible( bool Visible )
{
	if( Visible )
	{
		al_show_mouse_cursor( screen );
	} else {
		al_hide_mouse_cursor( screen );
	}
}

void Display::Render()
{
#ifndef PANDORA
	al_set_target_backbuffer( screen );
	al_clear_to_color( al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap( screenGameBuffer, 0, 0, gameSize.X, gameSize.Y, gameScreenLocation.X, gameScreenLocation.Y, gameScreenSize.X, gameScreenSize.Y, 0 );
	al_flip_display();

	al_set_target_bitmap( screenGameBuffer );
#else
	al_flip_display();
#endif
}

Vector2* Display::ScreenPointToGamePoint( Vector2* Point )
{
	return new Vector2( ScreenXToGameX( Point->X ), ScreenYToGameY( Point->Y ) );
}

float Display::ScreenXToGameX( float X )
{
	return ((X - gameScreenLocation.X) / gameScreenSize.X) * gameSize.X;
}

float Display::ScreenYToGameY( float Y )
{
	return ((Y - gameScreenLocation.Y) / gameScreenSize.Y) * gameSize.Y;
}
