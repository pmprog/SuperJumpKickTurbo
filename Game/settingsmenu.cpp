
#include "settingsmenu.h"
#include "settingskeyset.h"

void SettingsMenu::Begin()
{
	fontTitle = al_load_font( "resources/titlefont.ttf", 24, 0 );
	fontHeight = al_get_font_line_height( fontTitle );

	fontCredits = al_load_font( "resources/silkscreen.ttf", 16, 0 );
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
			FRAMEWORK->ProcessEvents();
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
			if( uiForm != nullptr )
			{
				FRAMEWORK->ProcessEvents();
				delete uiForm;
				uiForm = nullptr;
			}
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_UP && menuSelection > 0 )
		{
			menuSelection--;
			AUDIO->PlaySoundEffect( "resources/collision.wav" );
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_DOWN && menuSelection < 6 )
		{
			menuSelection++;
			AUDIO->PlaySoundEffect( "resources/collision.wav" );
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ENTER )
		{
			switch( menuSelection )
			{
				case 0:
					CreateVideoForm();
					break;
				case 1:
					CreateAudioForm();
					break;
				case 2:
					CreateInputForm();
					break;
				case 3:
					CreateNetworkForm();
					break;
				case 4:
					CreateSpecialsForm();
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

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy != nullptr && uiForm != nullptr )
	{
		if( e->Data.Forms.EventFlag == FormEventType::KeyDown )
		{
			if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_UP )
			{
				uiForm->FocusPrevious();
			}
			if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_DOWN )
			{
				uiForm->FocusNext();
			}
		}

		Form* f = e->Data.Forms.RaisedBy->GetForm();
		if( f->Name == "Video Settings" )
		{
			ProcessVideoFormEvents( e );
			return;
		}
		if( f->Name == "Audio Settings" )
		{
			ProcessAudioFormEvents( e );
			return;
		}
		if( f->Name == "Input Settings" )
		{
			ProcessInputFormEvents( e );
			return;
		}
		if( f->Name == "Network Settings" )
		{
			ProcessNetworkFormEvents( e );
			return;
		}
		if( f->Name == "Special Settings" )
		{
			ProcessSpecialsFormEvents( e );
			return;
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
	curY = DrawMenuItem( 2, curY, "Configure Input" );
	curY = DrawMenuItem( 3, curY, "Configure Network" );
	curY = DrawMenuItem( 4, curY, "Configure Specials" );
	curY = DrawMenuItem( 5, curY, "Save and Return" );
	curY = DrawMenuItem( 6, curY, "Cancel" );

	curY = DISPLAY->GetHeight() - (fontCreditsHeight * 11);
	curY = DrawMenuItem( 99, curY, "Credits:" );
	curY = DrawMenuItem( 100, curY, " Programming:" );
	curY = DrawMenuItem( 99, curY, "  Marq Watkin, Polymath Programming" );
	curY = DrawMenuItem( 99, curY, "  www.pmprog.co.uk" );
	curY = DrawMenuItem( 100, curY, " Music: " );
	curY = DrawMenuItem( 99, curY, "  Naildown55" );
	curY = DrawMenuItem( 99, curY, "  naildown55.newgrounds.com/audio/" );
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
	tew->Name = "Video.Width";
	tew->SetText( *FRAMEWORK->Settings->GetQuickStringValue( "Video.Width", "800" ) );

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
	teh->Name = "Video.Height";
	teh->SetText( *FRAMEWORK->Settings->GetQuickStringValue( "Video.Height", "480" ) );

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
	cb->Name = "Video.Fullscreen";
	cb->Checked = FRAMEWORK->Settings->GetQuickBooleanValue( "Video.Fullscreen", false );

	l = new Label( c, "Stretch to fill screen:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 160;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	cb = new CheckBox( c );
	cb->Location.X = 10;
	cb->Location.Y = l->Location.Y + l->Size.Y + 4;
	cb->Size.X = 24;
	cb->Size.Y = 24;
	cb->Name = "Video.ScaleMode";
	cb->Checked = ( FRAMEWORK->Settings->GetQuickIntegerValue( "Video.ScaleMode", 0 ) == 1 ? true : false );
	
	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;
	tb->Name = "Video.Ok";

	// Have to force process the Enter key events before I can set focus...
	FRAMEWORK->ProcessEvents();
	tew->Focus();
}

void SettingsMenu::ProcessVideoFormEvents(Event *e)
{
	if( (e->Data.Forms.RaisedBy->Name == "Video.Width" || e->Data.Forms.RaisedBy->Name == "Video.Height") && e->Data.Forms.EventFlag == FormEventType::TextChanged )
	{
		if( FRAMEWORK->Settings->IsNumber( ((TextEdit*)e->Data.Forms.RaisedBy)->GetText() ) )
		{
			FRAMEWORK->Settings->SetStringValue( e->Data.Forms.RaisedBy->Name, &((TextEdit*)e->Data.Forms.RaisedBy)->GetText() );
		}
	}
	if( e->Data.Forms.RaisedBy->Name == "Video.Fullscreen" && e->Data.Forms.EventFlag == FormEventType::CheckBoxChange )
	{
		FRAMEWORK->Settings->SetBooleanValue( e->Data.Forms.RaisedBy->Name, ((CheckBox*)e->Data.Forms.RaisedBy)->Checked );
	}
	if( e->Data.Forms.RaisedBy->Name == "Video.ScaleMode" && e->Data.Forms.EventFlag == FormEventType::CheckBoxChange )
	{
		FRAMEWORK->Settings->SetIntegerValue( e->Data.Forms.RaisedBy->Name, ( ((CheckBox*)e->Data.Forms.RaisedBy)->Checked ? 1 : 0 ) );
	}

	// Visual settings edited
	if( e->Data.Forms.RaisedBy->Name == "Video.Ok" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		AUDIO->PlaySoundEffect( "resources/collision.wav" );
		FRAMEWORK->ProcessEvents();
		delete uiForm;
		uiForm = nullptr;
	}
}

void SettingsMenu::CreateAudioForm()
{
	Control* c;
	Label* l;
	CheckBox* cbm;
	CheckBox* cbs;
	TextButton* tb;

	uiForm = new Form();
	uiForm->BackgroundColour = al_map_rgb( 0, 0, 0 );
	uiForm->Location.X = 200;
	uiForm->Location.Y = 150;
	uiForm->Size.X = 400;
	uiForm->Size.Y = 200;
	uiForm->Name= "Audio Settings";

	c = new Control( uiForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = uiForm->Size.X - 8;
	c->Size.Y = uiForm->Size.Y - 8;

	l = new Label( c, "Music On:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 10;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	cbm = new CheckBox( c );
	cbm->Location.X = 10;
	cbm->Location.Y = l->Location.Y + l->Size.Y + 4;
	cbm->Size.X = 24;
	cbm->Size.Y = 24;
	cbm->Name = "Audio.Music";
	cbm->Checked = FRAMEWORK->Settings->GetQuickBooleanValue( "Audio.Music", true );

	l = new Label( c, "Sound FX On:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 60;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	cbs = new CheckBox( c );
	cbs->Location.X = 10;
	cbs->Location.Y = l->Location.Y + l->Size.Y + 4;
	cbs->Size.X = 24;
	cbs->Size.Y = 24;
	cbs->Name = "Audio.Sound";
	cbs->Checked = FRAMEWORK->Settings->GetQuickBooleanValue( "Audio.Sound", true );
	
	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;
	tb->Name = "Audio.Ok";

	cbm->Focus();
}

void SettingsMenu::ProcessAudioFormEvents(Event *e)
{
	if( (e->Data.Forms.RaisedBy->Name == "Audio.Music" || e->Data.Forms.RaisedBy->Name == "Audio.Sound") && e->Data.Forms.EventFlag == FormEventType::CheckBoxChange )
	{
		FRAMEWORK->Settings->SetBooleanValue( e->Data.Forms.RaisedBy->Name, ((CheckBox*)e->Data.Forms.RaisedBy)->Checked );
	}

	// Visual settings edited
	if( e->Data.Forms.RaisedBy->Name == "Audio.Ok" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		AUDIO->PlaySoundEffect( "resources/collision.wav" );
		FRAMEWORK->ProcessEvents();
		delete uiForm;
		uiForm = nullptr;
	}
}

void SettingsMenu::CreateInputForm()
{
	Control* c;
	Label* l;
	TextButton* tb;
	TextButton* tbfirst;
	std::string* keyname;

	int currenty = 10;
	int rowheight = fontUI->GetFontHeight() + 14;
	int btnoffset = -1;
	int btnheight = rowheight - 2;

	uiForm = new Form();
	uiForm->BackgroundColour = al_map_rgb( 0, 0, 0 );
	uiForm->Location.X = 200;
	uiForm->Location.Y = 100;
	uiForm->Size.X = 400;
	uiForm->Size.Y = 300;
	uiForm->Name= "Input Settings";

	c = new Control( uiForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = uiForm->Size.X - 8;
	c->Size.Y = uiForm->Size.Y - 8;

	l = new Label( c, "Player 1:", fontUI );
	l->Location.X = 10;
	l->Location.Y = currenty;
	l->Size.X = 380;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Bottom;

	currenty += rowheight;

	l = new Label( c, "Jump:", fontUI );
	l->Location.X = 20;
	l->Location.Y = currenty;
	l->Size.X = 60;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Centre;

	keyname = new std::string( al_keycode_to_name( FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Jump", ALLEGRO_KEY_LSHIFT ) ) );
	tb = new TextButton( c, *keyname, fontUI );
	tb->Location.X = 85;
	tb->Location.Y = currenty - btnoffset;
	tb->Size.X = c->Size.X - 90;
	tb->Size.Y = btnheight;
	tb->Name = "Player1.Keyboard.Jump";

	tbfirst = tb;

	currenty += rowheight;

	l = new Label( c, "Kick:", fontUI );
	l->Location.X = 20;
	l->Location.Y = currenty;
	l->Size.X = 180;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Centre;

	keyname = new std::string( al_keycode_to_name( FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Kick", ALLEGRO_KEY_LCTRL ) ) );
	tb = new TextButton( c, *keyname, fontUI );
	tb->Location.X = 85;
	tb->Location.Y = currenty - btnoffset;
	tb->Size.X = c->Size.X - 90;
	tb->Size.Y = btnheight;
	tb->Name = "Player1.Keyboard.Kick";

	currenty += (rowheight * 1.5);

	l = new Label( c, "Player 2:", fontUI );
	l->Location.X = 10;
	l->Location.Y = currenty;
	l->Size.X = 380;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Bottom;

	currenty += rowheight;

	l = new Label( c, "Jump:", fontUI );
	l->Location.X = 20;
	l->Location.Y = currenty;
	l->Size.X = 60;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Centre;

	keyname = new std::string( al_keycode_to_name( FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Jump", ALLEGRO_KEY_RSHIFT ) ) );
	tb = new TextButton( c, *keyname, fontUI );
	tb->Location.X = 85;
	tb->Location.Y = currenty - btnoffset;
	tb->Size.X = c->Size.X - 90;
	tb->Size.Y = btnheight;
	tb->Name = "Player2.Keyboard.Jump";

	currenty += rowheight;

	l = new Label( c, "Kick:", fontUI );
	l->Location.X = 20;
	l->Location.Y = currenty;
	l->Size.X = 180;
	l->Size.Y = rowheight;
	l->TextVAlign = VerticalAlignment::Centre;

	keyname = new std::string( al_keycode_to_name( FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Kick", ALLEGRO_KEY_RCTRL ) ) );
	tb = new TextButton( c, *keyname, fontUI );
	tb->Location.X = 85;
	tb->Location.Y = currenty - btnoffset;
	tb->Size.X = c->Size.X - 90;
	tb->Size.Y = btnheight;
	tb->Name = "Player2.Keyboard.Kick";

	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;
	tb->Name = "Input.Ok";

	// Have to force process the Enter key events before I can set focus...
	FRAMEWORK->ProcessEvents();
	tbfirst->Focus();
}

void SettingsMenu::ProcessInputFormEvents(Event *e)
{
	if( e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{

		if( e->Data.Forms.RaisedBy->Name == "Input.Ok" )
		{
			AUDIO->PlaySoundEffect( "resources/collision.wav" );
			FRAMEWORK->ProcessEvents();
			delete uiForm;
			uiForm = nullptr;
		} else {
			FRAMEWORK->ProgramStages->Push( new SettingsKeySet(e->Data.Forms.RaisedBy->Name, (TextButton*)e->Data.Forms.RaisedBy) );
		}

	}
}

void SettingsMenu::CreateNetworkForm()
{
	Control* c;
	Label* l;
	TextEdit* ten;
	TextEdit* tep;
	TextButton* tb;

	uiForm = new Form();
	uiForm->BackgroundColour = al_map_rgb( 0, 0, 0 );
	uiForm->Location.X = 200;
	uiForm->Location.Y = 150;
	uiForm->Size.X = 400;
	uiForm->Size.Y = 200;
	uiForm->Name= "Network Settings";

	c = new Control( uiForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = uiForm->Size.X - 8;
	c->Size.Y = uiForm->Size.Y - 8;

	l = new Label( c, "Your Name:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 10;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	ten = new TextEdit( c, "", fontUI );
	ten->AllowTab = false;
	ten->Location.X = 10;
	ten->Location.Y = l->Location.Y + l->Size.Y + 4;
	ten->Size.X = 380;
	ten->Size.Y = fontUI->GetFontHeight();
	ten->Name = "Network.PlayerName";
	ten->SetText( *FRAMEWORK->Settings->GetQuickStringValue( "Network.PlayerName", "NetPlayer" ) );


	l = new Label( c, "UDP Port:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 60;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	tep = new TextEdit( c, "", fontUI );
	tep->AllowTab = false;
	tep->Location.X = 10;
	tep->Location.Y = l->Location.Y + l->Size.Y + 4;
	tep->Size.X = 380;
	tep->Size.Y = fontUI->GetFontHeight();
	tep->Name = "Network.Port";
	tep->SetText( *FRAMEWORK->Settings->GetQuickStringValue( "Network.Port", "9090" ) );
	
	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;
	tb->Name = "Network.Ok";

	// Have to force process the Enter key events before I can set focus...
	FRAMEWORK->ProcessEvents();
	ten->Focus();
}

void SettingsMenu::ProcessNetworkFormEvents(Event *e)
{
	if( e->Data.Forms.RaisedBy->Name == "Network.Port" && e->Data.Forms.EventFlag == FormEventType::TextChanged )
	{
		if( FRAMEWORK->Settings->IsNumber( ((TextEdit*)e->Data.Forms.RaisedBy)->GetText() ) )
		{
			FRAMEWORK->Settings->SetStringValue( e->Data.Forms.RaisedBy->Name, &((TextEdit*)e->Data.Forms.RaisedBy)->GetText() );
		}
	}
	if( e->Data.Forms.RaisedBy->Name == "Network.PlayerName" && e->Data.Forms.EventFlag == FormEventType::TextChanged )
	{
		FRAMEWORK->Settings->SetStringValue( e->Data.Forms.RaisedBy->Name, &((TextEdit*)e->Data.Forms.RaisedBy)->GetText() );
	}

	if( e->Data.Forms.RaisedBy->Name == "Network.Ok" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		AUDIO->PlaySoundEffect( "resources/collision.wav" );
		FRAMEWORK->ProcessEvents();
		delete uiForm;
		uiForm = nullptr;
	}
}

void SettingsMenu::CreateSpecialsForm()
{
	Control* c;
	Label* l;
	CheckBox* cbm;
	TextButton* tb;

	uiForm = new Form();
	uiForm->BackgroundColour = al_map_rgb( 0, 0, 0 );
	uiForm->Location.X = 200;
	uiForm->Location.Y = 150;
	uiForm->Size.X = 400;
	uiForm->Size.Y = 200;
	uiForm->Name= "Special Settings";

	c = new Control( uiForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = uiForm->Size.X - 8;
	c->Size.Y = uiForm->Size.Y - 8;

	l = new Label( c, "Show Collision Boxes:", fontUI );
	l->Location.X = 10;
	l->Location.Y = 10;
	l->Size.X = 380;
	l->Size.Y = fontUI->GetFontHeight();

	cbm = new CheckBox( c );
	cbm->Location.X = 10;
	cbm->Location.Y = l->Location.Y + l->Size.Y + 4;
	cbm->Size.X = 24;
	cbm->Size.Y = 24;
	cbm->Name = "Debug.ShowCollisionBoxes";
	cbm->Checked = FRAMEWORK->Settings->GetQuickBooleanValue( "Debug.ShowCollisionBoxes", false );
	
	tb = new TextButton( c, "Ok", fontUI );
	tb->Size.X = 80;
	tb->Size.Y = 40;
	tb->Location.X = c->Size.X - 90;
	tb->Location.Y = c->Size.Y - 50;
	tb->Name = "Specials.Ok";

	cbm->Focus();
}

void SettingsMenu::ProcessSpecialsFormEvents(Event *e)
{
	if( e->Data.Forms.RaisedBy->Name == "Debug.ShowCollisionBoxes" && e->Data.Forms.EventFlag == FormEventType::CheckBoxChange )
	{
		FRAMEWORK->Settings->SetBooleanValue( e->Data.Forms.RaisedBy->Name, ((CheckBox*)e->Data.Forms.RaisedBy)->Checked );
	}

	// Visual settings edited
	if( e->Data.Forms.RaisedBy->Name == "Specials.Ok" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		AUDIO->PlaySoundEffect( "resources/collision.wav" );
		FRAMEWORK->ProcessEvents();
		delete uiForm;
		uiForm = nullptr;
	}
}

