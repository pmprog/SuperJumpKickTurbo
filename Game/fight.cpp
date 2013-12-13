
#include "fight.h"
#include "../Framework/framework.h"

void Fight::Begin()
{
}

void Fight::Pause()
{
}

void Fight::Resume()
{
}

void Fight::Finish()
{
}

void Fight::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keysym.sym == SDLK_ESCAPE )
		{
			delete Framework::System->ProgramStages->Pop();
		}
	}
}

void Fight::Update()
{
}

void Fight::Render()
{
	spClearTarget( 0 );
}
