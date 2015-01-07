
#include "networkmenu.h"


NetworkMenu::NetworkMenu(Fighter::FighterController Controller)
{
	localController = Controller;

	hideForm = false;

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

	waitAngle = new Angle( 0 );
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

	if( !hideForm )
	{
		netForm->EventOccured(e);
	}

	if( e->Type == EVENT_FORM_INTERACTION )
	{
		if( e->Data.Forms.EventFlag == FormEventType::ButtonClick && e->Data.Forms.RaisedBy->Name == "Cancel" )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
		if( e->Data.Forms.EventFlag == FormEventType::ButtonClick && e->Data.Forms.RaisedBy->Name == "Start" )
		{
			if( te->GetText() == "" )
			{
				// Server
				Fighter::NetworkController = new Network( FRAMEWORK->Settings->GetQuickIntegerValue( "Network.Port", 9090 ) );
			} else {
				// Client
				Fighter::NetworkController = new Network( te->GetText(), FRAMEWORK->Settings->GetQuickIntegerValue( "Network.Port", 9090 ) );
			}
			hideForm = true;
		}
	}

	if( e->Type == EVENT_NETWORK_CONNECTION_REQUEST )
	{
		if( !Fighter::NetworkController->IsConnected() )
		{
			Fighter::NetworkController->AcceptConnection( e->Data.Network.Traffic.peer );
		}
	}
	if( e->Type == EVENT_NETWORK_CONNECTED )
	{
		// Fighter::NetworkController->Send("")
	}
	if( e->Type == EVENT_NETWORK_DISCONNECTED )
	{
		delete Fighter::NetworkController;
		Fighter::NetworkController = nullptr;

		hideForm = false;
	}
	if( e->Type == EVENT_NETWORK_PACKET_RECEIVED )
	{
		// TODO: Parse command
	}

}

void NetworkMenu::Update()
{
	FRAMEWORK->ProgramStages->Previous()->Update();
	if( !hideForm )
	{
		netForm->Update();
	}

	if( Fighter::NetworkController != nullptr )
	{
		Fighter::NetworkController->Update();
	}
	
	waitAngle->Add( 5 );
}

void NetworkMenu::Render()
{
	FRAMEWORK->ProgramStages->Previous()->Render();
	if( !hideForm )
	{
		netForm->Render();
	} else {
		al_draw_filled_rounded_rectangle( 360, 200, 440, 280, 6, 6, al_map_rgb( 128, 192, 128 ) );

		// waitAngle->Add( -40 );
		for( int i = 0; i <= 8; i++ )
		{
			al_draw_filled_pieslice( 400, 240, 30, waitAngle->ToRadians(), 0.2f, al_map_rgb( i * 30, i * 30, i * 30 ) );
			// waitAngle->Add( 5 );
		}

		
	}
}

bool NetworkMenu::IsTransition()
{
	return false;
}

