
#include "playerselect.h"
#include "arena.h"

PlayerSelect::PlayerSelect( bool Player1Active, bool Player2Active )
{
	p1active = Player1Active;
	if( p1active )
	{
		p1char = new Fighter( Fighter::LocalKeyboardP1, "resources/Akuma.txt", 0, false );
		p1char->Fighter_SetPosition( DISPLAY->GetWidth() / 3, 0 );
		p1char->Fighter_SetFacing( false );
		p1char->Fighter_SetState( Fighter::Idle );
	} else {
		p1char = 0;
	}

	p2active = Player2Active;
	if( p2active )
	{
		p2char = new Fighter( Fighter::LocalKeyboardP2, "resources/Akuma.txt", 0, false );
		p2char->Fighter_SetPosition( (DISPLAY->GetWidth() / 3) * 2, 0 );
		p2char->Fighter_SetFacing( true );
		p2char->Fighter_SetState( Fighter::Idle );
	} else {
		p2char = 0;
	}

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
}

void PlayerSelect::EventOccurred(Event *e)
{
}

void PlayerSelect::Update()
{
	if( p1char != 0 )
	{
		p1char->Fighter_Update( true );
	}
	if( p2char != 0 )
	{
		p2char->Fighter_Update( true );
	}
}

void PlayerSelect::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	if( p1char != 0 )
	{
		p1char->Fighter_Render( 0, 0 );
	}

	if( p2char != 0 )
	{
		p2char->Fighter_Render( 0, 0 );
	}
}

bool PlayerSelect::IsTransition()
{
	return false;
}
