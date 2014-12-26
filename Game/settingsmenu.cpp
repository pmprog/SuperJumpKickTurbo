
#include "settingsmenu.h"

void SettingsMenu::Begin()
{
	fontTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );
	fontHeight = al_get_font_line_height( fontTitle );

	menuSelection = 0;

	menuSelectedColour = al_map_rgb( 255, 255, 0 );
	menuItemColour = al_map_rgb( 220, 220, 220 );
}

void SettingsMenu::Pause()
{
}

void SettingsMenu::Resume()
{
}

void SettingsMenu::Finish()
{
	al_destroy_font( fontTitle );
}

void SettingsMenu::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_UP && menuSelection > 0 )
		{
			menuSelection--;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_DOWN && menuSelection < 6 )
		{
			menuSelection++;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ENTER )
		{
			switch( menuSelection )
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					FRAMEWORK->Settings->Save( "settings.cfg" );
					delete FRAMEWORK->ProgramStages->Pop();
					break;
				case 6:
					delete FRAMEWORK->Settings;
					FRAMEWORK->Settings = new ConfigFile( "settings.cfg" );
					delete FRAMEWORK->ProgramStages->Pop();
					break;
			}
		}
	}
}

void SettingsMenu::Update()
{
}

void SettingsMenu::Render()
{
	int curY = 20;

	al_clear_to_color( al_map_rgb( 32, 40, 32 ) );

	curY = DrawMenuItem( 0, curY, "Configure Video" );
	curY = DrawMenuItem( 1, curY, "Configure Audio" );
	curY = DrawMenuItem( 2, curY, "Configure Player 1 Input" );
	curY = DrawMenuItem( 3, curY, "Configure Player 2 Input" );
	curY = DrawMenuItem( 4, curY, "Configure Network" );
	curY = DrawMenuItem( 5, curY, "Save and Return" );
	curY = DrawMenuItem( 6, curY, "Cancel" );

}

bool SettingsMenu::IsTransition()
{
	return false;
}

int SettingsMenu::DrawMenuItem( int MenuID, int CurrentY, std::string Text )
{
	al_draw_text( fontTitle, ( menuSelection == MenuID ? menuSelectedColour : menuItemColour ), 20, CurrentY, ALLEGRO_ALIGN_LEFT, Text.c_str() );
	return CurrentY + fontHeight;
}