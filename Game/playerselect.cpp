
#include "playerselect.h"
#include "arena.h"
#include "menu.h"

std::string PlayerSelect::Player1Name = "Player 1";
std::string PlayerSelect::Player2Name = "Player 2";


PlayerSelect::PlayerSelect( Fighter::FighterController Player1Controls, Fighter::FighterController Player2Controls )
{
	datalists = new ConfigFile( "resources/data.txt" );

	imgBackground = al_load_bitmap( "resources/cloudback.png" );
	backgroundX = 0;

	p1charidx = 0;
	p1char = nullptr;
	if( Player1Controls != Fighter::FighterController::NoControls )
	{
		ConstructPlayer1( Player1Controls, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
	}

	p2charidx = 0;
	p2char = nullptr;
	if( Player2Controls != Fighter::FighterController::NoControls )
	{
		ConstructPlayer2( Player2Controls, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
	}

	fntName = al_load_ttf_font( "resources/titlefont.ttf", 32, 0 );
	fntNameHeight = al_get_font_line_height( fntName );
	fntControls = al_load_ttf_font( "resources/titlefont.ttf", 24, 0 );

	stageTime = 0;
	blinkOn = false;
}

void PlayerSelect::Begin()
{
}

void PlayerSelect::Pause()
{
}

void PlayerSelect::Resume()
{
}

void PlayerSelect::Finish()
{
	al_destroy_bitmap( imgBackground );
	al_destroy_font( fntName );
	al_destroy_font( fntControls );
}

void PlayerSelect::EventOccurred(Event *e)
{
	Fighter::FighterController source = Fighter::FighterController::NoControls;
	bool sourceisjump = false;
	bool transmitinput = false;

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Jump", ALLEGRO_KEY_LSHIFT ) )
		{
			source = Fighter::FighterController::LocalKeyboardP1;
			sourceisjump = true;
			transmitinput = true;
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Kick", ALLEGRO_KEY_LCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP1;
			sourceisjump = false;
			transmitinput = true;
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Jump", ALLEGRO_KEY_RSHIFT ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = true;
			transmitinput = true;
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Kick", ALLEGRO_KEY_RCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = false;
			transmitinput = true;
		}
	}

	if( e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
		Menu* menustage = (Menu*)FRAMEWORK->ProgramStages->Previous();

		// Setup joystick controls
		if( menustage->Player1Joystick != e->Data.Joystick.ID && menustage->Player2Joystick != e->Data.Joystick.ID )
		{
			if( menustage->Player1Joystick == -1 )
			{
				menustage->Player1Joystick = e->Data.Joystick.ID;
			} else if ( menustage->Player2Joystick == -1 ) {
				menustage->Player2Joystick = e->Data.Joystick.ID;
			}
		}

		if( e->Data.Joystick.ID == menustage->Player1Joystick )
		{
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Joystick.Jump", 0 ) )
			{
				source = Fighter::FighterController::LocalJoystickP1;
				sourceisjump = true;
				transmitinput = true;
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP1;
				sourceisjump = false;
				transmitinput = true;
			}
		}
		if( e->Data.Joystick.ID == menustage->Player2Joystick )
		{
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Jump", 0 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = true;
				transmitinput = true;
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = false;
				transmitinput = true;
			}
		}
	}

	GamePacket netpacket;

	if( e->Type == EVENT_NETWORK_PACKET_RECEIVED )
	{
		if( e->Data.Network.Traffic.packet->dataLength != sizeof( netpacket ) )
		{
#ifdef WRITE_LOG
			printf("Error: Invalid network packet length of %d, expecting %d", e->Data.Network.Traffic.packet->dataLength, sizeof( netpacket ) );
#endif
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
		memcpy((void*)&netpacket, e->Data.Network.Traffic.packet->data, e->Data.Network.Traffic.packet->dataLength );

		if( netpacket.Type == PACKET_TYPE_INPUT && (netpacket.Data.Input.JumpPressed || netpacket.Data.Input.KickPressed) )
		{
			source = Fighter::FighterController::NetworkClient;
			sourceisjump = netpacket.Data.Input.JumpPressed;
		}

	}

	// Network game
	if( p1char->Controller == Fighter::FighterController::NetworkClient || p2char->Controller == Fighter::FighterController::NetworkClient )
	{

		// Player disconnected
		if( e->Type == EVENT_NETWORK_DISCONNECTED )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		// Send player local input
		if( transmitinput && source != Fighter::FighterController::NetworkClient )
		{
			netpacket.Type = PACKET_TYPE_INPUT;
			netpacket.FrameCount = 0;
			netpacket.Data.Input.JumpPressed = sourceisjump;
			netpacket.Data.Input.KickPressed = !sourceisjump;

			Fighter::NetworkController->Send( (void*)&netpacket, sizeof(netpacket), true );
		}
	}


	if( source != Fighter::FighterController::NoControls )
	{
		int activeplayer = GetPlayerWithControls( source );
		switch( activeplayer )
		{
			case 1:
				if( sourceisjump )
				{
					p1charidx = (p1charidx + 1) % datalists->GetArraySize( "Characters" );
					ConstructPlayer1( source, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
				} else {
					p1char->Fighter_SetState( Fighter::FighterStates::Victor );
				}
				break;

			case 2:
				if( sourceisjump )
				{
					p2charidx = (p2charidx + 1) % datalists->GetArraySize( "Characters" );
					ConstructPlayer2( source, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
				} else {
					p2char->Fighter_SetState( Fighter::FighterStates::Victor );
				}
				break;

			case 0:
				if( p1char == nullptr )
				{
					ConstructPlayer1( source, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
				} else if ( p2char == nullptr ) {
					ConstructPlayer2( source, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
				}
				break;
		}

	}

}

void PlayerSelect::Update()
{
	Arena* ingame;

	stageTime = (stageTime + 1) % FRAMEWORK->GetFramesPerSecond();
	if( stageTime == 0 )
	{
		blinkOn = !blinkOn;
	}

	if( p1char != 0 )
	{
		p1char->Fighter_Update( true );
	}
	if( p2char != 0 )
	{
		p2char->Fighter_Update( true );
	}

	if( p1char != nullptr && p2char != nullptr )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor && p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Both players have selected their fighters
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

	if( p1char != nullptr && p2char == nullptr )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer2( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

	if( p1char == nullptr && p2char != nullptr )
	{
		if( p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer1( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
			return;
		}
	}

	backgroundX--;
	if( backgroundX <= -800 )
	{
		backgroundX = 0;
	}

	if( Fighter::NetworkController != nullptr )
	{
		Fighter::NetworkController->Update();
	}
}

void PlayerSelect::Render()
{
	// al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	al_draw_bitmap( imgBackground, backgroundX, 0, 0 );
	al_draw_bitmap( imgBackground, backgroundX + 800, 0, 0 );

	al_draw_text( fntControls, al_map_rgb( 128, 255, 128 ), 400, 10, ALLEGRO_ALIGN_CENTRE, "Jump: Change Character" );
	al_draw_text( fntControls, al_map_rgb( 128, 255, 128 ), 400, 40, ALLEGRO_ALIGN_CENTRE, "Kick: Ready!" );


	if( p1char != 0 )
	{
		p1char->Fighter_Render( 0, 0 );
		al_draw_text( fntName, al_map_rgb( 255, 255, 255 ), 10, 90, ALLEGRO_ALIGN_LEFT, p1char->CharacterName.c_str() );
		al_draw_text( fntName, al_map_rgb( 255, 255, 255 ), 10, 470 - fntNameHeight, ALLEGRO_ALIGN_LEFT, p1char->PlayerName.c_str() );
	} else {
		if( !blinkOn )
		{
			al_draw_text( fntName, al_map_rgb( 255, 255, 0 ), 220, 200, ALLEGRO_ALIGN_CENTRE, "Press" );
			al_draw_text( fntName, al_map_rgb( 255, 255, 0 ), 220, 240, ALLEGRO_ALIGN_CENTRE, "Button!" );
		}
		
	}

	if( p2char != 0 )
	{
		p2char->Fighter_Render( 0, 0 );
		al_draw_text( fntName, al_map_rgb( 255, 255, 255 ), 790, 90, ALLEGRO_ALIGN_RIGHT, p2char->CharacterName.c_str() );
		al_draw_text( fntName, al_map_rgb( 255, 255, 255 ), 790, 470 - fntNameHeight, ALLEGRO_ALIGN_RIGHT, p2char->PlayerName.c_str() );
	} else {
		if( !blinkOn )
		{
			al_draw_text( fntName, al_map_rgb( 255, 255, 0 ), 580, 200, ALLEGRO_ALIGN_CENTRE, "Press" );
			al_draw_text( fntName, al_map_rgb( 255, 255, 0 ), 580, 240, ALLEGRO_ALIGN_CENTRE, "Button!" );
		}
	}
}

bool PlayerSelect::IsTransition()
{
	return false;
}

void PlayerSelect::ConstructPlayer1( Fighter::FighterController Controller, std::string Character )
{
	p1char = new Fighter( Controller, Character.c_str(), 0, false );
	p1char->Fighter_SetPosition( DISPLAY->GetWidth() / 4, 0 );
	p1char->Fighter_SetState( Fighter::Idle );
	p1char->Fighter_SetFacing( false );
	p1char->PlayerName = PlayerSelect::Player1Name;
}

void PlayerSelect::ConstructPlayer2( Fighter::FighterController Controller, std::string Character )
{
	p2char = new Fighter( Controller, Character.c_str(), 0, true );
	p2char->Fighter_SetPosition( (DISPLAY->GetWidth() / 4) * 3, 0 );
	p2char->Fighter_SetState( Fighter::Idle );
	p2char->Fighter_SetFacing( true );
	p2char->PlayerName = PlayerSelect::Player2Name;
}

int PlayerSelect::GetPlayerWithControls( Fighter::FighterController Controller )
{
	if( p1char != nullptr )
	{
		if( p1char->Controller == Controller )
		{
			return 1;
		}
	}
	if( p2char != nullptr )
	{
		if( p2char->Controller == Controller )
		{
			return 2;
		}
	}
	return 0;
}
