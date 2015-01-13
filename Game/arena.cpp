
#include "arena.h"
#include "roundover.h"
#include "matchover.h"
#include "menu.h"
#include "roundcountin.h"

int Arena::Player1TotalWins = 0;
int Arena::Player2TotalWins = 0;


Arena::Arena()
{
	DebugReverse = false;

	Background = al_load_bitmap( "resources/japan.png" );
	ArenaWidth = al_get_bitmap_width(Background);

	DemoMode = true;

	Player1 = 0;
	Player2 = 0;

	AUDIO->PlayMusic( "resources/naildown55-demo_riffs_4.ogg", true );

	CollisionGraphics = new SpriteSheet( "resources/collision.png", 72, 72 );
	CollisionAnimation = new Animation( CollisionGraphics, false, 5 );
	CollisionAnimation->AddFrame( 0 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 0 );
	CollisionAnimation->AddFrame( 0 );

	RoundMarkers = new SpriteSheet( "resources/roundmarkers.png", 48, 48 );
}

Arena::Arena( std::string LocationImage, Fighter* P1, Fighter* P2 )
{
	DebugReverse = false;

	Background = al_load_bitmap( LocationImage.c_str() );
	ArenaWidth = al_get_bitmap_width(Background);

	DemoMode = false;

	Player1 = P1;
	Player2 = P2;

	AUDIO->PlayMusic( "resources/naildown55-demo_riffs_4.ogg", true );

	CollisionGraphics = new SpriteSheet( "resources/collision.png", 72, 72 );
	CollisionAnimation = new Animation( CollisionGraphics, false, 5 );
	CollisionAnimation->AddFrame( 0 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 1 );
	CollisionAnimation->AddFrame( 0 );
	CollisionAnimation->AddFrame( 0 );

	RoundMarkers = new SpriteSheet( "resources/roundmarkers.png", 48, 48 );
}

Arena::~Arena()
{
	al_destroy_bitmap( Background );
	delete CollisionAnimation;
	delete CollisionGraphics;

	delete RoundMarkers;
}

void Arena::Begin()
{
	fntTimer = al_load_ttf_font( "resources/titlefont.ttf", 48, 0 );
	Player1Wins = 0;
	Player2Wins = 0;
	ResetArena();
}

void Arena::Pause()
{
}

void Arena::Resume()
{
	//ResetArena();
}

void Arena::Finish()
{
	al_destroy_font( fntTimer );
	AUDIO->PlayMusic( "resources/naildown55-demo_riffs_3_loopedit.ogg", true );
}

void Arena::EventOccurred(Event *e)
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
			transmitinput = ( GetPlayerWithControls( source ) != nullptr );
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Keyboard.Kick", ALLEGRO_KEY_LCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP1;
			sourceisjump = false;
			transmitinput = ( GetPlayerWithControls( source ) != nullptr );
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Jump", ALLEGRO_KEY_RSHIFT ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = true;
			transmitinput = ( GetPlayerWithControls( source ) != nullptr );
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Keyboard.Kick", ALLEGRO_KEY_RCTRL ) )
		{
			source = Fighter::FighterController::LocalKeyboardP2;
			sourceisjump = false;
			transmitinput = ( GetPlayerWithControls( source ) != nullptr );
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
				transmitinput = ( GetPlayerWithControls( source ) != nullptr );
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP1;
				sourceisjump = false;
				transmitinput = ( GetPlayerWithControls( source ) != nullptr );
			}
		}
		if( e->Data.Joystick.ID == menustage->Player2Joystick )
		{
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Jump", 0 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = true;
				transmitinput = ( GetPlayerWithControls( source ) != nullptr );
			}
			if( e->Data.Joystick.Button == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Joystick.Kick", 1 ) )
			{
				source = Fighter::FighterController::LocalJoystickP2;
				sourceisjump = false;
				transmitinput = ( GetPlayerWithControls( source ) != nullptr );
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
			// TODO: Check if we need a rollback!
			// State_Load( netpacket.FrameCount )

			source = Fighter::FighterController::NetworkClient;
			sourceisjump = netpacket.Data.Input.JumpPressed;
		}

	}

	// Network game
	if( Player1->Controller == Fighter::FighterController::NetworkClient || Player2->Controller == Fighter::FighterController::NetworkClient )
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
			netpacket.FrameCount = RoundFrameCount;
			netpacket.Data.Input.JumpPressed = sourceisjump;
			netpacket.Data.Input.KickPressed = !sourceisjump;

			Fighter::NetworkController->Send( (void*)&netpacket, sizeof(netpacket), true );
		}
	}

	if( source != Fighter::FighterController::NoControls )
	{
		Fighter* f = GetPlayerWithControls( source );

		if( f == nullptr )
		{
			// A new challenger?

		} else if( sourceisjump ) {
			f->Fighter_JumpPressed();
		} else {
			f->Fighter_KickPressed();
		}
	}

}

void Arena::Update()
{
	if( Fighter::NetworkController != nullptr )
	{
		Fighter::NetworkController->Update();
	}

	if( DebugReverse )
	{
		if( RoundFrameCount > 2 )
		{
			State_Load( RoundFrameCount - 2 );
			FixCameraPosition();
		}
		return;
	}

	RoundFrameCount++;

	// Update collision animations (Ignore slow mode)
	if( CollisionsAt[0] != nullptr || CollisionsAt[1] != nullptr )
	{
		CollisionAnimation->Update();
		if( CollisionAnimation->HasEnded() )
		{
			for( int i = 0; i < 2; i++ )
			{
				if( CollisionsAt[i] != nullptr )
				{
					delete CollisionsAt[i];
					CollisionsAt[i] = nullptr;
				}
			}
		}
	}

	if( SlowMode > 0 )
	{
		SlowModeDelay = (SlowModeDelay + 1) % (SLOWMODE_DELAY_FRAMES * SlowMode);
		if( SlowModeDelay != 0 )
		{
			return;
		} else {
			SlowMode--;
		}
	}

	if( !DisableTimer )
	{
		TickRoundClock();
	}

	// Update players
	Player1->Fighter_Update( false );
	Player2->Fighter_Update( false );

	// Update to knockdown after checking both players (double-ko possibility)
	if( Player1->FighterHit )
	{
		Player1->Fighter_SetState( Fighter::Knockdown );
		SlowMode = 8;
		DisableTimer = true;
	}
	if( Player2->FighterHit )
	{
		Player2->Fighter_SetState( Fighter::Knockdown );
		SlowMode = 8;
		DisableTimer = true;
	}

	if( Player1->Fighter_GetState() == Fighter::Idle && (Player2->Fighter_GetState() == Fighter::Knockdown || Player2->Fighter_GetState() == Fighter::Loser) )
	{
		Player1->Fighter_SetState( Fighter::Victor );

		if( !DemoMode )
		{
			Player1Wins++;
		}

		SlowMode = 0;
		// Let Round/Match Over stages update the animations, but don't spawn millions of stages
		if( FRAMEWORK->ProgramStages->Current() == this )
		{
			FRAMEWORK->ProgramStages->Push( new RoundOver( 1 ) );
		}
	}
	if( Player2->Fighter_GetState() == Fighter::Idle && (Player1->Fighter_GetState() == Fighter::Knockdown || Player1->Fighter_GetState() == Fighter::Loser) )
	{
		Player2->Fighter_SetState( Fighter::Victor );
		if( !DemoMode )
		{
			Player2Wins++;
		}
		SlowMode = 0;
		// Let Round/Match Over stages update the animations, but don't spawn millions of stages
		if( FRAMEWORK->ProgramStages->Current() == this )
		{
			FRAMEWORK->ProgramStages->Push( new RoundOver( 2 ) );
		}
	}
	if( Player1->Fighter_GetState() == Fighter::Loser && Player2->Fighter_GetState() == Fighter::Loser )
	{
		// Draw
		SlowMode = 0;
		// Let Round/Match Over stages update the animations, but don't spawn millions of stages
		if( FRAMEWORK->ProgramStages->Current() == this )
		{
			FRAMEWORK->ProgramStages->Push( new RoundOver( 0 ) );
		}
	}

	if( Player1->Fighter_GetPosition()->X < Player2->Fighter_GetPosition()->X )
	{
		Player1->Fighter_SetFacing( false );
		Player2->Fighter_SetFacing( true );
	} else {
		Player2->Fighter_SetFacing( false );
		Player1->Fighter_SetFacing( true );
	}

	// Update camera
	if( CamXMove != 0 )
	{
		Camera.X += 2.0f * CamXMove;
	}
	if( CamYMove != 0 )
	{
		Camera.Y += 2.0f * CamYMove;
	}

	FixCameraPosition();

	// Lock players in screen
	switch( Player1->Fighter_GetState() )
	{
		case Fighter::BackJump:
		case Fighter::Kick:
		case Fighter::Super:
		case Fighter::Ultra:
			if( Player1->Fighter_GetPosition()->X < Camera.X )
			{
				Player1->Fighter_GetPosition()->X = Camera.X;
			}
			if( Player1->Fighter_GetPosition()->X > Camera.X + DISPLAY->GetWidth() )
			{
				Player1->Fighter_GetPosition()->X = Camera.X + DISPLAY->GetWidth();
			}
			break;
	}
	// Lock players in screen
	switch( Player2->Fighter_GetState() )
	{
		case Fighter::BackJump:
		case Fighter::Kick:
		case Fighter::Super:
		case Fighter::Ultra:
			if( Player2->Fighter_GetPosition()->X < Camera.X )
			{
				Player2->Fighter_GetPosition()->X = Camera.X;
			}
			if( Player2->Fighter_GetPosition()->X > Camera.X + DISPLAY->GetWidth() )
			{
				Player2->Fighter_GetPosition()->X = Camera.X + DISPLAY->GetWidth();
			}
			break;
	}

}

void Arena::Render()
{
	// Fix background drawing
	al_draw_bitmap( Background, -Camera.X, DISPLAY->GetHeight() - al_get_bitmap_height(Background) + Camera.Y, 0 );

	// Draw Fighters

	switch( Player2->Fighter_GetState() )
	{
		case Fighter::FighterStates::Knockdown:
		case Fighter::FighterStates::Loser:
		case Fighter::FighterStates::Floored:
			Player2->Fighter_Render( Camera.X, Camera.Y );
			Player1->Fighter_Render( Camera.X, Camera.Y );
			break;
		default:
			Player1->Fighter_Render( Camera.X, Camera.Y );
			Player2->Fighter_Render( Camera.X, Camera.Y );
			break;
	}

	// Draw collision graphics
	for( int i = 0; i < 2; i++ )
	{
		if( CollisionsAt[i] != nullptr )
		{
			CollisionAnimation->DrawFrame( CollisionsAt[i]->X - 36 - Camera.X, CollisionsAt[i]->Y - 36 + Camera.Y );
		}
	}

	for( int r = 0; r < 4; r++ )
	{
		RoundMarkers->DrawSprite( ( Player1Wins > r ? 1 : 0 ), 10 + (r * 52), 10 );
		RoundMarkers->DrawSprite( ( Player2Wins > r ? 1 : 0 ), 790 - (r * 52) - 48, 10 );
	}

	// Draw timer
	al_draw_textf( fntTimer, al_map_rgb( 0, 0, 0 ), (DISPLAY->GetWidth() / 2) + 4, 14, ALLEGRO_ALIGN_CENTRE, "%d", CountdownTimer );
	al_draw_textf( fntTimer, al_map_rgb( 255, 255, 0 ), DISPLAY->GetWidth() / 2, 10, ALLEGRO_ALIGN_CENTRE, "%d", CountdownTimer );
}

bool Arena::IsTransition()
{
	return false;
}

void Arena::ResetArena()
{
	RoundFrameCount = 0;
	CountdownTimer = ROUND_TIME;
	CountdownTimerTicker = 0;
	DisableTimer = false;

	if( DemoMode )
	{
		if( Player1 != 0 )
		{
			delete Player1;
		}
		if( Player2 != 0 )
		{
			delete Player2;
		}

		int p1char = rand() % 3;
		int p1skin = rand() % 2;
		int p2char = rand() % 3;
		int p2skin = rand() % 2;
		
		if( p1char == p2char && p1skin == p2skin )
		{
			p2skin = 1 - p2skin;
		}

		switch( p1char )
		{
			case 0:
				Player1 = new Fighter( Fighter::CPU_Easy, "resources/ryu.txt", this, (p1skin == 0) );
				break;
			case 1:
				Player1 = new Fighter( Fighter::CPU_Easy, "resources/akuma.txt", this, (p1skin == 0) );
				break;
			case 2:
				Player1 = new Fighter( Fighter::CPU_Easy, "resources/bison.txt", this, (p1skin == 0) );
				break;
		}
		switch( p2char )
		{
			case 0:
				Player2 = new Fighter( Fighter::CPU_Easy, "resources/ryu.txt", this, (p2skin == 0) );
				break;
			case 1:
				Player2 = new Fighter( Fighter::CPU_Easy, "resources/akuma.txt", this, (p2skin == 0) );
				break;
			case 2:
				Player2 = new Fighter( Fighter::CPU_Easy, "resources/bison.txt", this, (p2skin == 0) );
				break;
		}
	}

	Player1->State_Clear();
	Player1->Fighter_SetPosition( al_get_bitmap_width(Background) / 3, 0 );
	Player1->Fighter_SetState( Fighter::Idle );
	Player1->Fighter_SetFacing( false );

	Player2->State_Clear();
	Player2->Fighter_SetPosition( (al_get_bitmap_width(Background) / 3) * 2, 0 );
	Player2->Fighter_SetState( Fighter::Idle );
	Player2->Fighter_SetFacing( true );

	Camera.X = (ArenaWidth / 2) - (DISPLAY->GetWidth() / 2);
	Camera.Y = 0;
	CamXMove = 0;
	CamYMove = 0;

	SlowMode = 0;
	SlowModeDelay = 0;

	CollisionsAt[0] = nullptr;
	CollisionsAt[1] = nullptr;

	for( int i = 0; i < ROUND_TIME; i++ )
	{
		ClockRoundFrameCount[i] = 0;
	}

	FRAMEWORK->ProgramStages->Push( new RoundCountIn() );
}

Fighter* Arena::GetOpponent(Fighter* Current)
{
	if( Player1 == Current )
	{
		return Player2;
	} else {
		return Player1;
	}
}

bool Arena::State_Load(uint64_t FrameCount)
{
	RoundFrameCount = FrameCount;

	if( !Player1->State_Load( RoundFrameCount ) || !Player2->State_Load( RoundFrameCount ) )
	{
		return false;
	}

	for( int i = 0; i < ROUND_TIME; i++ )
	{
		if( ClockRoundFrameCount[i] > 0 && ClockRoundFrameCount[i] <= FrameCount )
		{
			CountdownTimer = i + 1;
			CountdownTimerTicker = FrameCount - ClockRoundFrameCount[i];
			break;
		} else {
			ClockRoundFrameCount[i] = 0;
		}
	}

	return true;
}

void Arena::TickRoundClock()
{
	if( CountdownTimer > 0 )
	{
		CountdownTimerTicker = (CountdownTimerTicker + 1) % FRAMEWORK->GetFramesPerSecond();
		if( CountdownTimerTicker == 0 )
		{
			CountdownTimer--;
			// Save frame count of clock
			if( CountdownTimer > 0 )
			{
				ClockRoundFrameCount[CountdownTimer - 1] = RoundFrameCount;
			}
		}
	}
	if( CountdownTimer == 0 )
	{
		Player1->Fighter_SetState( Fighter::Knockdown );
		Player2->Fighter_SetState( Fighter::Knockdown );
		SlowMode = 2;
		DisableTimer = true;
	}
}

void Arena::FixCameraPosition()
{
	int xmax = Maths::Max( Player1->Fighter_GetPosition()->X, Player2->Fighter_GetPosition()->X );
	int xmin = Maths::Min( Player1->Fighter_GetPosition()->X, Player2->Fighter_GetPosition()->X );
	Camera.X = xmin + ((xmax - xmin) / 2) - (DISPLAY->GetWidth() / 2);

	int ymax = Maths::Max( Player1->Fighter_GetPosition()->Y, Player2->Fighter_GetPosition()->Y );
	int ymin = Maths::Min( Player1->Fighter_GetPosition()->Y, Player2->Fighter_GetPosition()->Y );
	Camera.Y = ymin + ((ymax - ymin) / 2) - 68;

	if( Camera.X < 0 )
	{
		Camera.X = 0;
	}
	if( Camera.Y < 0 )
	{
		Camera.Y = 0;
	}
	if( Camera.X > ArenaWidth - DISPLAY->GetWidth() )
	{
		Camera.X = ArenaWidth - DISPLAY->GetWidth();
	}
	if( Camera.Y > al_get_bitmap_height(Background) - DISPLAY->GetHeight() )
	{
		Camera.Y = al_get_bitmap_height(Background) - DISPLAY->GetHeight();
	}
}

Fighter* Arena::GetPlayerWithControls( Fighter::FighterController Controller )
{
	if( Player1->Controller == Controller )
	{
		return Player1;
	}
	if( Player2->Controller == Controller )
	{
		return Player2;
	}
	return nullptr;
}

void Arena::AddCollisionAt( Vector2* Location )
{
	if( CollisionsAt[0] == nullptr )
	{
		CollisionsAt[0] = Location;
	} else {
		CollisionsAt[1] = Location;
	}
	CollisionAnimation->Start();
}
