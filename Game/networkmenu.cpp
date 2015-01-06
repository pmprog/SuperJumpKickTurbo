
#include "networkmenu.h"


NetworkMenu::NetworkMenu(Fighter::FighterController Controller)
{
	localController = Controller;

	fontForm = new TTFFont( "resources/titlefont.ttf", 16 );

	netForm = new Form();
	netForm->Location.X = 80;
	netForm->Location.Y = 190;
	netForm->Size.X = 640;
	netForm->Size.Y = 100;
	netForm->BackgroundColour = al_map_rgb( 0, 0, 0 );

	Control* c = new Control( netForm );
	c->Location.X = 4;
	c->Location.Y = 4;
	c->Size.X = netForm->Size.X - 8;
	c->Size.Y = netForm->Size.Y - 8;
	c->BackgroundColour = al_map_rgb( 48, 54, 48 );

	Label* l = new Label( c, "Hostname:", fontForm );
	l->BackgroundColour = c->BackgroundColour;
	l->Location.X = 4;
	l->Location.Y = 4;
	l->Size.X = fontForm->GetFontWidth( "Hostname: " );
	l->Size.Y = fontForm->GetFontHeight();

	te = new TextEdit( c, "", fontForm );
	te->BackgroundColour = c->BackgroundColour;
	te->ForegroundColour = al_map_rgb( 255, 220, 0 );
	te->Location.X = l->Size.X + 6;
	te->Location.Y = 4;
	te->Size.X = c->Size.X - te->Location.X - 6;
	te->Size.Y = fontForm->GetFontHeight();

	l = new Label( c, "Leave hostname blank to act as the server", fontForm );
	l->BackgroundColour = c->BackgroundColour;
	l->Location.X = 4;
	l->Location.Y = fontForm->GetFontHeight() + 6;
	l->Size.X = fontForm->GetFontWidth( l->GetText() );
	l->Size.Y = fontForm->GetFontHeight();

	TextButton* b = new TextButton( c, "Start", fontForm );
	b->Name = "Start";
	b->Size.X = fontForm->GetFontWidth( "  Button  " );
	b->Size.Y = fontForm->GetFontHeight() + 6;
	b->Location.X = c->Size.X - ((b->Size.X + 4) * 2);
	b->Location.Y = c->Size.Y - b->Size.Y - 4;

	b = new TextButton( c, "Cancel", fontForm );
	b->Name = "Cancel";
	b->Size.X = fontForm->GetFontWidth( "  Button  " );
	b->Size.Y = fontForm->GetFontHeight() + 6;
	b->Location.X = c->Size.X - b->Size.X - 4;
	b->Location.Y = c->Size.Y - b->Size.Y - 4;

}

NetworkMenu::~NetworkMenu()
{
	FRAMEWORK->ProcessEvents();
	delete netForm;
	netForm = nullptr;
}

void NetworkMenu::Begin()
{
	FRAMEWORK->ProcessEvents();
	te->Focus();
}

void NetworkMenu::Pause()
{
}

void NetworkMenu::Resume()
{
}

void NetworkMenu::Finish()
{
}

void NetworkMenu::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN && e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
	{
		delete FRAMEWORK->ProgramStages->Pop();
		return;
	}

	netForm->EventOccured(e);

	if( e->Type == EVENT_FORM_INTERACTION )
	{
		if( e->Data.Forms.EventFlag == FormEventType::ButtonClick && e->Data.Forms.RaisedBy->Name == "Cancel" )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
		if( e->Data.Forms.EventFlag == FormEventType::ButtonClick && e->Data.Forms.RaisedBy->Name == "Start" )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
	}

}

void NetworkMenu::Update()
{
	FRAMEWORK->ProgramStages->Previous()->Update();
	netForm->Update();
}

void NetworkMenu::Render()
{
	FRAMEWORK->ProgramStages->Previous()->Render();
	netForm->Render();
}

bool NetworkMenu::IsTransition()
{
	return false;
}

