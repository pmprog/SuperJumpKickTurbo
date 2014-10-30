
#include "arena.h"

void Arena::Begin()
{
	Player1 = new Fighter( "resources/akuma.txt" );
	Player2 = new Fighter( "resources/akuma.txt" );
	Background = al_load_bitmap( "resources/japan.png" );
	Camera.X = (al_get_bitmap_width(Background) / 2) - (DISPLAY->GetWidth() / 2);
	Camera.Y = 0;

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
	if( Camera.X > al_get_bitmap_width(Background) - DISPLAY->GetWidth() )
	{
		Camera.X = al_get_bitmap_width(Background) - DISPLAY->GetWidth();
	}
	if( Camera.Y > al_get_bitmap_height(Background) - DISPLAY->GetHeight() )
	{
		Camera.Y = al_get_bitmap_height(Background) - DISPLAY->GetHeight();
	}

	Player1->Fighter_Update();
	Player2->Fighter_Update();
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
