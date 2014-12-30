
#include "menu.h"
#include "arena.h"
#include "settingsmenu.h"
#include "playerselect.h"

int Menu::Player1Joystick = -1;
int Menu::Player2Joystick = -1;

void Menu::Begin()
{
	backgroundX = 0;
	imgSuper = al_load_bitmap( "resources/super.png" );
	imgJumpKick = al_load_bitmap( "resources/jumpkick.png" );
	imgTurbo = new SpriteSheet( "resources/turbo.png", 204, 64 );
	imgBackground = al_load_bitmap( "resources/cloudback.png" );

	fntTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );

	menuTime = 0;
	menuSelection = 0;

	menuSelectedColour = al_map_rgb( 255, 255, 0 );
	menuItemColour = al_map_rgb( 220, 220, 220 );

	TitleFighters[0] = new Fighter( Fighter::NoControls, "resources/akuma.txt", nullptr, false );
	TitleFighters[0]->Fighter_SetPosition( 0, 400 );
	TitleFighters[0]->Fighter_SetFacing( false );

	TitleFighters[1] = new Fighter( Fighter::NoControls, "resources/ryu.txt", nullptr, false );
	TitleFighters[1]->Fighter_SetPosition( 0, 265 );
	TitleFighters[1]->Fighter_SetFacing( false );

	TitleFighters[2] = new Fighter( Fighter::NoControls, "resources/bison.txt", nullptr, false );
	TitleFighters[2]->Fighter_SetPosition( 0, 510 );
	TitleFighters[2]->Fighter_SetFacing( false );

	AUDIO->PlayMusic( "resources/naildown55-demo_riffs_3_loopedit.ogg", true );
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
	bool rushedintro = false;
	bool activateoption = false;
	bool wasp1active = true;
	Fighter::FighterController source1 = Fighter::FighterController::NoControls;
	Fighter::FighterController source2 = Fighter::FighterController::NoControls;

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( menuTime < 270 )
		{
			menuTime = 270;
			rushedintro = true;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			FRAMEWORK->ShutdownFramework();
			return;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_UP && menuSelection > 0 )
		{
			menuSelection--;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_DOWN && menuSelection < 4 )
		{
			menuSelection++;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ENTER && !rushedintro )
		{
			activateoption = true;
			source1 = Fighter::FighterController::LocalKeyboardP1;
		}
		if( (e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Jump", ALLEGRO_KEY_LSHIFT ) || e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Kick", ALLEGRO_KEY_LCTRL )) && !rushedintro )
		{
			activateoption = true;
			source1 = Fighter::FighterController::LocalKeyboardP1;
		}
		if( (e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Jump", ALLEGRO_KEY_RSHIFT ) || e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Kick", ALLEGRO_KEY_RCTRL )) && !rushedintro )
		{
			activateoption = true;
			source2 = Fighter::FighterController::LocalKeyboardP2;
		}
	}

	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
		if( menuTime < 270 )
		{
			menuTime = 270;
			rushedintro = true;
		} else {
			activateoption = true;
		}

		if( Player1Joystick != -1 )
		{
			Player1Joystick = e->Data.Joystick.ID;
		} else if( Player2Joystick != -1 ) {
			Player2Joystick = e->Data.Joystick.ID;
		}

		if( Player1Joystick == e->Data.Joystick.ID )
		{
			source1 = Fighter::FighterController::LocalJoystickP1;
		}
		if( Player2Joystick == e->Data.Joystick.ID )
		{
			source2 = Fighter::FighterController::LocalJoystickP2;
		}

	}

	if( e->Type == EVENT_JOYSTICK_AXIS )
	{
		if( e->Data.Joystick.Axis == 1 )
		{
			if( e->Data.Joystick.Position < 0 && menuSelection > 0 )
			{
				menuSelection--;
			}
			if( e->Data.Joystick.Position > 0 && menuSelection < 4 )
			{
				menuSelection++;
			}
		}

		if( Player1Joystick != -1 )
		{
			Player1Joystick = e->Data.Joystick.ID;
		} else if( Player2Joystick != -1 ) {
			Player2Joystick = e->Data.Joystick.ID;
			wasp1active = false;
		}
	}

	if( activateoption )
	{
		switch( menuSelection )
		{
			case 0:
				// Player Select
				FRAMEWORK->ProgramStages->Push( new PlayerSelect( source1, source2 ) );
				break;
			case 1:
				// Open Network Hub
				break;
			case 2:
				// Demo
				FRAMEWORK->ProgramStages->Push( new Arena() );
				break;
			case 3:
				FRAMEWORK->ProgramStages->Push( new SettingsMenu() );
				break;
			case 4:
				AUDIO->StopMusic();
				FRAMEWORK->ShutdownFramework();
				break;
		}
	}

}

void Menu::Update()
{
	menuTime++;
	if( menuTime == 275 )
	{
		TitleFighters[0]->Fighter_SetState( Fighter::FighterStates::Kick );
		TitleFighters[1]->Fighter_SetState( Fighter::FighterStates::Kick );
		TitleFighters[2]->Fighter_SetState( Fighter::FighterStates::Kick );
	}
	TitleFighters[0]->Fighter_Update( true );
	TitleFighters[1]->Fighter_Update( true );
	TitleFighters[2]->Fighter_Update( true );

	backgroundX--;
	if( backgroundX <= -800 )
	{
		backgroundX = 0;
	}
}

void Menu::Render()
{
	// al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	al_draw_bitmap( imgBackground, backgroundX, 0, 0 );
	al_draw_bitmap( imgBackground, backgroundX + 800, 0, 0 );


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

		al_draw_text( fntTitle, ( menuSelection == 0 ? menuSelectedColour : menuItemColour ), DISPLAY->GetWidth() - 20, 310, ALLEGRO_ALIGN_RIGHT, "Arcade" );
		al_draw_text( fntTitle, ( menuSelection == 1 ? menuSelectedColour : menuItemColour ), DISPLAY->GetWidth() - 20, 340, ALLEGRO_ALIGN_RIGHT, "Network" );
		al_draw_text( fntTitle, ( menuSelection == 2 ? menuSelectedColour : menuItemColour ), DISPLAY->GetWidth() - 20, 370, ALLEGRO_ALIGN_RIGHT, "Demo" );
		al_draw_text( fntTitle, ( menuSelection == 3 ? menuSelectedColour : menuItemColour ), DISPLAY->GetWidth() - 20, 400, ALLEGRO_ALIGN_RIGHT, "Settings" );
		al_draw_text( fntTitle, ( menuSelection == 4 ? menuSelectedColour : menuItemColour ), DISPLAY->GetWidth() - 20, 430, ALLEGRO_ALIGN_RIGHT, "Quit" );



	}

	TitleFighters[0]->Fighter_Render( 300, 0 );
	TitleFighters[1]->Fighter_Render( 300, 0 );
	TitleFighters[2]->Fighter_Render( 300, 0 );
}

bool Menu::IsTransition()
{
	return false;
}
