
#include "fight.h"
#include "../Framework/framework.h"
#include "fighter_akuma.h"

void Fight::Begin()
{
	ScreenScale = (Framework::System->GetDisplayHeight() / 240.0f);

	ArenaWidth = Framework::System->GetDisplayWidth() * 1.8f;
	ArenaHeight = Framework::System->GetDisplayWidth() * 1.5f;
	CameraX = ArenaWidth / 2;
	CameraY = 0;

	PlayerA = new Akuma();
	PlayerA->Initialise( ScreenScale );
	PlayerA->Fighter_SetPosition( (ArenaWidth / 2) - (Framework::System->GetDisplayWidth() / 4), 0 );

	PlayerB = new Akuma();
	PlayerB->Initialise( ScreenScale );
	PlayerB->Fighter_SetFacing( true );
	PlayerB->Fighter_SetPosition( (ArenaWidth / 2) + (Framework::System->GetDisplayWidth() / 4), 0 );
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
		if( e->Data.Keyboard.keysym.sym == SDLK_INSERT )
		{
			PlayerA->Fighter_JumpPressed();
		}
		if( e->Data.Keyboard.keysym.sym == SDLK_HOME )
		{
			PlayerA->Fighter_KickPressed();
		}
		if( e->Data.Keyboard.keysym.sym == SDLK_PAGEUP )
		{
			PlayerA->Fighter_SuperPressed();
		}
		if( e->Data.Keyboard.keysym.sym == SDLK_DELETE )
		{
			PlayerB->Fighter_JumpPressed();
		}
		if( e->Data.Keyboard.keysym.sym == SDLK_END )
		{
			PlayerB->Fighter_KickPressed();
		}
		if( e->Data.Keyboard.keysym.sym == SDLK_PAGEDOWN )
		{
			PlayerB->Fighter_SuperPressed();
		}
	}
}

void Fight::Update()
{
	PlayerA->Fighter_Update();
	PlayerB->Fighter_Update();

	if( PlayerA->Fighter_GetPosition()->X < PlayerB->Fighter_GetPosition()->X )
	{
		PlayerA->Fighter_SetFacing( false );
		PlayerB->Fighter_SetFacing( true );
	} else {
		PlayerA->Fighter_SetFacing( true );
		PlayerB->Fighter_SetFacing( false );
	}
}

void Fight::Render()
{
	spClearTarget( 0 );
	PlayerA->Fighter_Render( CameraX - (Framework::System->GetDisplayWidth() / 2), CameraY );
	PlayerB->Fighter_Render( CameraX - (Framework::System->GetDisplayWidth() / 2), CameraY );
}
