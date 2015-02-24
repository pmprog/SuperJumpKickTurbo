
#include "settingskeyset.h"

SettingsKeySet::SettingsKeySet( std::string KeyName, TextButton* Source )
{
	keyname = KeyName;
	sourcebutton = Source;
}

void SettingsKeySet::Begin()
{
	fontTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );
	fontHeight = al_get_font_line_height( fontTitle );
}

void SettingsKeySet::Pause()
{
}

void SettingsKeySet::Resume()
{
}

void SettingsKeySet::Finish()
{
	al_destroy_font( fontTitle );
}

void SettingsKeySet::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode != ALLEGRO_KEY_ESCAPE )
		{
			FRAMEWORK->Settings->SetIntegerValue( keyname, e->Data.Keyboard.KeyCode );
			int keyval = FRAMEWORK->Settings->GetQuickIntegerValue( sourcebutton->Name, -1 );
			if( keyval >= 0 )
			{
				std::string* keyname = new std::string( al_keycode_to_name( keyval ) );
				sourcebutton->SetText( *keyname );
			}
		}
		delete FRAMEWORK->ProgramStages->Pop();
		return;
	}
}

void SettingsKeySet::Update()
{
}

void SettingsKeySet::Render()
{
	FRAMEWORK->ProgramStages->Previous()->Render();
	al_draw_filled_rectangle( 0, 280, 800, 286, al_map_rgb( 0, 0, 0 ) );
	al_draw_filled_rectangle( 0, 200, 800, 280, al_map_rgb( 0, 0, 255 ) );
	al_draw_text( fontTitle, al_map_rgb( 0, 0, 0 ), 404, 244 - (fontHeight / 2), ALLEGRO_ALIGN_CENTRE, "Press Key, or ESC to Cancel" );
	al_draw_text( fontTitle, al_map_rgb( 255, 255, 255 ), 400, 240 - (fontHeight / 2), ALLEGRO_ALIGN_CENTRE, "Press Key, or ESC to Cancel" );
}

bool SettingsKeySet::IsTransition()
{
	return false;
}
