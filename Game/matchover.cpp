
#include "matchover.h"

void MatchOver::Begin()
{
}

void MatchOver::Pause()
{
}

void MatchOver::Resume()
{
}

void MatchOver::Finish()
{
}

void MatchOver::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		delete FRAMEWORK->ProgramStages->Pop();
	}
}

void MatchOver::Update()
{
}

void MatchOver::Render()
{
	al_draw_filled_rectangle( 20, 20, 50, 50, al_map_rgb(220, 100, 220 ) );
}

bool MatchOver::IsTransition()
{
	return false;
}
