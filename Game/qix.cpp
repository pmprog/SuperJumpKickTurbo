
#include "qix.h"
#include "../Framework/framework.h"

void Qix::Begin()
{
}

void Qix::Pause()
{
}

void Qix::Resume()
{
}

void Qix::Finish()
{
}

void Qix::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keysym.sym == SDLK_ESCAPE )
		{
			delete Framework::System->ProgramStages->Pop();
		}
	}
}

void Qix::Update()
{
}

void Qix::Render()
{
	// spFloodFill( x, Framework::System->GetDisplayHeight() - 48, spGetRGB(200, 200, 200) );
}
