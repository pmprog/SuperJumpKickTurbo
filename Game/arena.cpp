
#include "arena.h"
#include "roundover.h"
#include "matchover.h"

Arena::Arena()
{
	Background = al_load_bitmap( "resources/japan.png" );
	arenaWidth = al_get_bitmap_width(Background);

	Player1 = new Fighter( "resources/akuma.txt", arenaWidth, false );
	Player2 = new Fighter( "resources/bison.txt", arenaWidth, false );
}

Arena::Arena( std::string LocationImage, Fighter* P1, Fighter* P2 )
{
	Background = al_load_bitmap( LocationImage.c_str() );
	arenaWidth = al_get_bitmap_width(Background);

	Player1 = P1;
	Player2 = P2;
}

Arena::~Arena()
{
	al_destroy_bitmap( Background );
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
	ResetArena();
}

void Arena::Finish()
{
	al_destroy_font( fntTimer );
}

void Arena::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_LSHIFT )
		{
			Player1->Fighter_JumpPressed();
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_LCTRL )
		{
			Player1->Fighter_KickPressed();
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_RSHIFT )
		{
			Player2->Fighter_JumpPressed();
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_RCTRL )
		{
			Player2->Fighter_KickPressed();
		}

	}

}

void Arena::Update()
{

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
		if( CountdownTimer > 0 )
		{
			CountdownTimerTicker = (CountdownTimerTicker + 1) % FRAMEWORK->GetFramesPerSecond();
			if( CountdownTimerTicker == 0 )
			{
				CountdownTimer--;
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


	// Update players
	Player1->Fighter_Update( this );
	Player2->Fighter_Update( this );

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
		Player1Wins++;
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
		Player2Wins++;
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

	int xmax = Maths::Max( Player1->Fighter_GetPosition()->X, Player2->Fighter_GetPosition()->X );
	int xmin = Maths::Min( Player1->Fighter_GetPosition()->X, Player2->Fighter_GetPosition()->X );
	Camera.X = xmin + ((xmax - xmin) / 2) - (DISPLAY->GetWidth() / 2);

	int ymax = Maths::Max( Player1->Fighter_GetPosition()->Y, Player2->Fighter_GetPosition()->Y );
	int ymin = Maths::Min( Player1->Fighter_GetPosition()->Y, Player2->Fighter_GetPosition()->Y );
	Camera.Y = ymin + ((ymax - ymin) / 2);

	if( Camera.X < 0 )
	{
		Camera.X = 0;
	}
	if( Camera.Y < 0 )
	{
		Camera.Y = 0;
	}
	if( Camera.X > arenaWidth - DISPLAY->GetWidth() )
	{
		Camera.X = arenaWidth - DISPLAY->GetWidth();
	}
	if( Camera.Y > al_get_bitmap_height(Background) - DISPLAY->GetHeight() )
	{
		Camera.Y = al_get_bitmap_height(Background) - DISPLAY->GetHeight();
	}

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

	Player1->Fighter_Render( Camera.X, Camera.Y );
	Player2->Fighter_Render( Camera.X, Camera.Y );

	al_draw_textf( fntTimer, al_map_rgb( 0, 0, 0 ), (DISPLAY->GetWidth() / 2) + 4, 14, ALLEGRO_ALIGN_CENTRE, "%d", CountdownTimer );
	al_draw_textf( fntTimer, al_map_rgb( 255, 255, 0 ), DISPLAY->GetWidth() / 2, 10, ALLEGRO_ALIGN_CENTRE, "%d", CountdownTimer );

}

bool Arena::IsTransition()
{
	return false;
}

void Arena::ResetArena()
{
	CountdownTimer = 20;
	CountdownTimerTicker = 0;
	DisableTimer = false;

	Player1->Fighter_SetPosition( al_get_bitmap_width(Background) / 3, 0 );
	Player1->Fighter_SetFacing( false );
	Player1->Fighter_SetState( Fighter::Idle );
	Player2->Fighter_SetPosition( (al_get_bitmap_width(Background) / 3) * 2, 0 );
	Player2->Fighter_SetFacing( true );
	Player2->Fighter_SetState( Fighter::Idle );

	Camera.X = (arenaWidth / 2) - (DISPLAY->GetWidth() / 2);
	Camera.Y = 0;
	CamXMove = 0;
	CamYMove = 0;

	SlowMode = 0;
	SlowModeDelay = 0;
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
