
#include "boot.h"
#include "../Framework/framework.h"
#include "menu.h"

void BootUp::Begin()
{
	bootBarSize = 0;
	bootBarAdjust = (Framework::System->GetDisplayWidth() / (FRAMES_PER_SECOND * 1.5));
	logoSprite = new SpriteSheet( spLoadSurface("Resource/pmprog.png"), 204, 200 );
	logoFadeIn = 0;
}

void BootUp::Pause()
{
}

void BootUp::Resume()
{
}

void BootUp::Finish()
{
}

void BootUp::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.keysym.sym == SDLK_ESCAPE )
		{
			delete Framework::System->ProgramStages->Pop();
		} else {
			StartGame();
		}
	}
}

void BootUp::Update()
{
	if( logoFadeIn < 128 )
	{
		logoFadeIn += 4;
	}
	if( bootBarSize < Framework::System->GetDisplayWidth() )
	{
		bootBarSize += bootBarAdjust;
	} else {
		StartGame();
		return;
	}
}

void BootUp::Render()
{
	spClearTarget( 0 );
	spSetHorizontalOrigin( SP_CENTER );
	spSetVerticalOrigin( SP_CENTER );

	logoSprite->DrawSprite( 0, Framework::System->GetDisplayWidth() / 2, Framework::System->GetDisplayHeight() / 2, logoFadeIn / 128.0f, logoFadeIn / 128.0f, 0 );
	spRectangle( Framework::System->GetDisplayWidth() / 2, Framework::System->GetDisplayHeight() - 12 , -1, bootBarSize, 8, spGetRGB( 255, 120, 0 ) );
}

void BootUp::StartGame()
{
	delete Framework::System->ProgramStages->Pop();
	// TODO: Boot stage
	Framework::System->ProgramStages->Push( (Stage*)new Menu() );
}
