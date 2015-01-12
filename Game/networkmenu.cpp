
#include "networkmenu.h"
#include "playerselect.h"

NetworkMenu::NetworkMenu(Fighter::FighterController Controller)
{
	localController = Controller;

	hideForm = false;
	isNetworkHost = false;
	setP1Name = false;
	setP2Name = false;

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
	byte nullterm = 0;

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
				isNetworkHost = true;
			} else {
				// Client
				Fighter::NetworkController = new Network( te->GetText(), FRAMEWORK->Settings->GetQuickIntegerValue( "Network.Port", 9090 ) );
				isNetworkHost = false;
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
		std::string* plyName = FRAMEWORK->Settings->GetQuickStringValue( "Network.PlayerName", "NetPlayer" );
		Memory* plyNamePacket = new Memory( 0 );
		plyNamePacket->AppendData( (void*)plyName->c_str(), plyName->length() );
		plyNamePacket->AppendData( (void*)&nullterm, 1 );
		Fighter::NetworkController->Send( plyNamePacket, true );
		delete plyNamePacket;

		if( isNetworkHost )
		{
			PlayerSelect::Player1Name.clear();
			PlayerSelect::Player1Name.append( *plyName );
			setP1Name = true;
		} else {
			PlayerSelect::Player2Name.clear();
			PlayerSelect::Player2Name.append( *plyName );
			setP2Name = true;
		}
		delete plyName;

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
		//Memory* packetData = new Memory( 0 );
		//packetData->AppendData( e->Data.Network.Traffic.packet->data, e->Data.Network.Traffic.packet->dataLength );
		std::string* plyName = new std::string( (char*)e->Data.Network.Traffic.packet->data );
		if( !isNetworkHost )
		{
			PlayerSelect::Player1Name.clear();
			PlayerSelect::Player1Name.append( *plyName );
			setP1Name = true;
		} else {
			PlayerSelect::Player2Name.clear();
			PlayerSelect::Player2Name.append( *plyName );
			setP2Name = true;
		}
		delete plyName;
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

	if( setP1Name && setP2Name )
	{
		std::string temp = PlayerSelect::Player1Name;
		temp = PlayerSelect::Player2Name;
		PlayerSelect* plyslc = new PlayerSelect( ( isNetworkHost ? localController : Fighter::FighterController::NetworkClient ), ( !isNetworkHost ? localController : Fighter::FighterController::NetworkClient ) );
		delete FRAMEWORK->ProgramStages->Pop();
		FRAMEWORK->ProgramStages->Push( plyslc );
		return;
	}

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

