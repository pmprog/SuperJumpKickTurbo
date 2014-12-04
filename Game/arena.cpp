
#include "arena.h"

void Arena::Begin()
{
	Background = al_load_bitmap( "resources/japan.png" );
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

		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_LEFT )
		{
			CamXMove = -1;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_RIGHT )
		{
			CamXMove = 1;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_UP )
		{
			CamYMove = 1;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_DOWN )
		{
			CamYMove = -1;
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
	if( e->Type == EVENT_KEY_UP )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_LEFT || e->Data.Keyboard.KeyCode == ALLEGRO_KEY_RIGHT )
		{
			CamXMove = 0;
		}
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_UP || e->Data.Keyboard.KeyCode == ALLEGRO_KEY_DOWN )
		{
			CamYMove = 0;
		}
	}
}

void Arena::Update()
{
	if( CamXMove != 0 )
	{
		Camera.X += 2.0f * CamXMove;
	}
	if( CamYMove != 0 )
	{
		Camera.Y += 2.0f * CamYMove;
	}

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

	Player1->Fighter_Update();
	Player2->Fighter_Update();

	if( Player1->Fighter_GetPosition()->X < Player2->Fighter_GetPosition()->X )
	{
		Player1->Fighter_SetFacing( false );
		Player2->Fighter_SetFacing( true );
	} else {
		Player2->Fighter_SetFacing( false );
		Player1->Fighter_SetFacing( true );
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
