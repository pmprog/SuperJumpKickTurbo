
#include "settingsmenu.h"

void SettingsMenu::Begin()
{
	fontTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );
	fontHeight = al_get_font_line_height( fontTitle );

	fontCredits = al_load_font( "resources/titlefont.ttf", 12, 0 );
	fontCreditsHeight = al_get_font_line_height( fontCredits );

	fontUI = new TTFFont( "resources/titlefont.ttf", 16 );

	menuSelection = 0;

	menuSelectedColour = al_map_rgb( 255, 255, 0 );
	menuItemColour = al_map_rgb( 220, 220, 220 );

	uiForm = nullptr;
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
	al_destroy_font( fontCredits );
	delete fontUI;
}

void SettingsMenu::EventOccurred(Event *e)
{
	if( uiForm != nullptr )
	{
		uiForm->EventOccured(e);

		if( e->Type == EVENT_KEY_DOWN && e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete uiForm;
			uiForm = nullptr;
			return;
		}

		if( e->Handled )
		{
			return;
		}
	}

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
					CreateVideoForm();
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
					FRAMEWORK->Settings->Save( "settings.cfg", false );
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
	if( uiForm != nullptr )
	{
		uiForm->Update();
	}
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

	curY = DISPLAY->GetHeight() - (fontCreditsHeight * 12);
	curY = DrawMenuItem( 99, curY, "Credits:" );
	curY = DrawMenuItem( 100, curY, " Programming:" );
	curY = DrawMenuItem( 99, curY, "  Marq Watkin, Polymath Programming" );
	curY = DrawMenuItem( 99, curY, "  www.pmprog.co.uk" );
	curY = DrawMenuItem( 100, curY, " Music: " );
	curY = DrawMenuItem( 99, curY, "  Naildown55" );
	curY = DrawMenuItem( 99, curY, "  naildown55.newgrounds.com" );
	curY = DrawMenuItem( 100, curY, " Graphics: " );
	curY = DrawMenuItem( 99, curY, "  Ripped from SNK vs Capcom" );

	if( uiForm != nullptr )
	{
		uiForm->Render();
	}
}

bool SettingsMenu::IsTransition()
{
	return false;
}

int SettingsMenu::DrawMenuItem( int MenuID, int CurrentY, std::string Text )
{
	al_draw_text( ( MenuID < 99 ? fontTitle : fontCredits ), ( menuSelection == MenuID || MenuID == 100 ? menuSelectedColour : menuItemColour ), 20, CurrentY, ALLEGRO_ALIGN_LEFT, Text.c_str() );
	return CurrentY + ( MenuID < 99 ? fontHeight : fontCreditsHeight );
}

void SettingsMenu::CreateVideoForm()
{
	Control* c;
	Label* l;
	TextEdit* tew;
	TextEdit* teh;
	CheckBox* cb;
	TextButton* tb;

	uiForm = new Form();
	uiForm->BackgroundColour = al_map_rgb( 0, 0, 0 );
	uiForm->Location.X = 200;
	uiForm->Location.Y = 100;
	uiForm->Size.X = 400;
	uiForm->Size.Y = 300;
	uiForm->Name= "Video Settings";

	c = new Control( uiForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = uiForm->Size.X - 8;
	c->Size.Y = uiForm->Size.Y - 8;

	l = new Label( c, "Screen Width:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 10;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	tew = new TextEdit( c, "", fontUI );
	tew->AllowTab = false;
	tew->Location.X = 10;
	tew->Location.Y = l->Location.Y + l->Size.Y + 4;
	tew->Size.X = 380;
	tew->Size.Y = fontUI->GetFontHeight();
	tew->Name = "Screen.Width";

	l = new Label( c, "Screen Height:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 60;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	teh = new TextEdit( c, "", fontUI );
	teh->AllowTab = false;
	teh->Location.X = 10;
	teh->Location.Y = l->Location.Y + l->Size.Y + 4;
	teh->Size.X = 380;
	teh->Size.Y = fontUI->GetFontHeight();
	teh->Name = "Screen.Height";

	l = new Label( c, "Fullscreen:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 110;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	cb = new CheckBox( c );
	cb->Location.X = 10;
	cb->Location.Y = l->Location.Y + l->Size.Y + 4;
	cb->Size.X = 24;
	cb->Size.Y = 24;
	cb->Name = "Screen.Fullscreen";
	
	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;

	tew->Focus();
	tew->BeginEdit();
}
