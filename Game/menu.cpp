
#include "menu.h"
#include "arena.h"

void Menu::Begin()
{
	imgSuper = al_load_bitmap( "resources/super.png" );
	imgJumpKick = al_load_bitmap( "resources/jumpkick.png" );
	imgTurbo = new SpriteSheet( "resources/turbo.png", 204, 64 );

	fntTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );

	menuTime = 0;

	TitleFighter = new Fighter( "resources/akuma.txt", nullptr, DISPLAY->GetWidth() * 2, false );
	TitleFighter->Fighter_SetPosition( 1000, 400 );
	TitleFighter->Fighter_SetFacing( true );
}

void Menu::Pause()
{
}

void Menu::Resume()
{
}

void Menu::Finish()
{
	al_destroy_bitmap( imgSuper );
	al_destroy_bitmap( imgJumpKick );
	delete imgTurbo;

	al_destroy_font( fntTitle );
}

void Menu::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( menuTime < 270 )
		{
			menuTime = 270;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			FRAMEWORK->ShutdownFramework();
			return;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ENTER )
		{
			// Testing
			FRAMEWORK->ProgramStages->Push( new Arena() );
		}
	}
}

void Menu::Update()
{
	menuTime++;
	if( menuTime == 275 )
	{
		TitleFighter->Fighter_SetState( Fighter::FighterStates::Kick );
	}
	TitleFighter->Fighter_Update( true );
}

void Menu::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );


	if( menuTime > 73 )
	{
		al_draw_filled_rectangle( 0, 35, DISPLAY->GetWidth(), 65 + al_get_bitmap_height( imgSuper ), al_map_rgb( 36, 64, 36 ) );
		al_draw_bitmap( imgSuper, (DISPLAY->GetWidth() / 2) - (al_get_bitmap_width(imgSuper) / 2), 50, 0 );
	} else if( menuTime > 10 ) {
		al_draw_filled_rectangle( 0, 35, DISPLAY->GetWidth(), 65 + al_get_bitmap_height( imgSuper ), al_map_rgb( (menuTime - 10) / 2, (menuTime - 10), (menuTime - 10) / 2 ) );
		al_draw_tinted_bitmap( imgSuper, al_map_rgba( 255, 255, 255, (menuTime - 10) * 4 ), (DISPLAY->GetWidth() / 2) - (al_get_bitmap_width(imgSuper) / 2), 50, 0 );
	}

	if( menuTime > 220 )
	{
		al_draw_bitmap( imgJumpKick, (DISPLAY->GetWidth() / 2) - (al_get_bitmap_width(imgJumpKick) / 2), 50, 0 );
	}

	if( menuTime > 260 )
	{
		int turboX = (int)((DISPLAY->GetWidth() / 4.0f) * 3.0f) - (imgTurbo->GetFrame(0)->Width / 2); // - imgTurbo->GetFrame(0)->Width - 30;
		int turboY = 65 + al_get_bitmap_height( imgSuper ) - imgTurbo->GetFrame(0)->Height - 5;

		imgTurbo->DrawSprite( 0, turboX, turboY );

		if( (menuTime % 800) >= 300 && (menuTime % 800) <= 300 + (imgTurbo->GetFrame(0)->Height / 4) )
		{
			int overlaypos = (imgTurbo->GetFrame(0)->Height / 4) - ((menuTime % 800) - 300);
			imgTurbo->DrawSpritePortion( 1, turboX, turboY + (overlaypos * 4), 0, overlaypos, imgTurbo->GetFrame(0)->Width, 12 );
		}

		al_draw_text( fntTitle, al_map_rgb( 255, 255, 0 ), 10, 330, 0, "Arcade" );
		al_draw_text( fntTitle, al_map_rgb( 128, 128, 128 ), 10, 360, 0, "Network" );
		al_draw_text( fntTitle, al_map_rgb( 255, 255, 255 ), 10, 390, 0, "Quit" );

	}

	TitleFighter->Fighter_Render( 0, 0 );
}

bool Menu::IsTransition()
{
	return false;
}
