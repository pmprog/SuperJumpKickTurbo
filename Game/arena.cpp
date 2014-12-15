
#include "arena.h"

void Arena::Begin()
{
	Background = al_load_bitmap( "resources/england.png" );
	arenaWidth = al_get_bitmap_width(Background);

	Camera.X = (arenaWidth / 2) - (DISPLAY->GetWidth() / 2);
	Camera.Y = 0;

	Player1 = new Fighter( "resources/akuma.txt", arenaWidth );
	Player2 = new Fighter( "resources/ryu.txt", arenaWidth );

	Player1->Fighter_SetPosition( al_get_bitmap_width(Background) / 3, 0 );
	Player1->Fighter_SetFacing( false );
	Player2->Fighter_SetPosition( (al_get_bitmap_width(Background) / 3) * 2, 0 );
	Player2->Fighter_SetFacing( true );

	CamXMove = 0;
	CamYMove = 0;
}

void Arena::Pause()
{
}

void Arena::Resume()
{
}

void Arena::Finish()
{
	al_destroy_bitmap( Background );
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

	// Update players
	Player1->Fighter_Update( this );
	Player2->Fighter_Update( this );

	// Update to knockdown after checking both players (double-ko possibility)
	if( Player1->FighterHit )
	{
		Player1->Fighter_SetState( Fighter::Knockdown );
	}
	if( Player2->FighterHit )
	{
		Player2->Fighter_SetState( Fighter::Knockdown );
	}

	if( Player1->Fighter_GetState() == Fighter::Idle && (Player2->Fighter_GetState() == Fighter::Knockdown || Player2->Fighter_GetState() == Fighter::Loser) )
	{
		Player1->Fighter_SetState( Fighter::Victor );
	}
	if( Player2->Fighter_GetState() == Fighter::Idle && (Player1->Fighter_GetState() == Fighter::Knockdown || Player1->Fighter_GetState() == Fighter::Loser) )
	{
		Player2->Fighter_SetState( Fighter::Victor );
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
}

bool Arena::IsTransition()
{
	return false;
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
