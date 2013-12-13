
#include "charselect.h"
#include "../Framework/framework.h"

void CharacterSelect::Begin()
{
}

void CharacterSelect::Pause()
{
}

void CharacterSelect::Resume()
{
}

void CharacterSelect::Finish()
{
}

void CharacterSelect::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keysym.sym == SDLK_ESCAPE )
		{
			delete Framework::System->ProgramStages->Pop();
		}
	}
}

void CharacterSelect::Update()
{
}

void CharacterSelect::Render()
{
	spClearTarget( 0 );
}
