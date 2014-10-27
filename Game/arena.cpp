
#include "arena.h"

void Arena::Begin()
{
	Player1 = new Fighter( "resources/akuma.txt" );
	Player2 = new Fighter( "resources/akuma.txt" );
}

void Arena::Pause()
{
}

void Arena::Resume()
{
}

void Arena::Finish()
{
}

void Arena::EventOccurred(Event *e)
{
}

void Arena::Update()
{
}

void Arena::Render()
{
	Player1->Fighter_Render( 0, 0 );
	Player2->Fighter_Render( 0, 0 );
}

bool Arena::IsTransition()
{
	return false;
}
