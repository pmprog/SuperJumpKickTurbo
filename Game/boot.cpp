
#include "boot.h"
#include "../Framework/framework.h"
#include "../Framework/Display/display.h"
#include "../Framework/Network/download.h"

void BootUp::Begin()
{
	DISPLAY->SetTitle("Super JumpKick Turbo");
	bootBarSize = 0;
	bootBarAdjust = (DISPLAY->GetWidth() / (FRAMEWORK->GetFramesPerSecond() * 1.5f));
	logoSprite = new SpriteSheet( "resources/pmprog.png", 204, 200 );
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
  delete logoSprite;
}

void BootUp::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
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
	if( bootBarSize < DISPLAY->GetWidth() )
	{
		bootBarSize += bootBarAdjust;
	} else {
		StartGame();
		return;
	}
}

void BootUp::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	int qrtWidth = DISPLAY->GetWidth() / 2;
	int hlfHeight = DISPLAY->GetHeight() / 2;
	float scale = logoFadeIn / 128.0f;

	logoSprite->DrawSprite( 0, qrtWidth - ((logoSprite->GetFrame( 0 )->Width * scale) / 2), hlfHeight - ((logoSprite->GetFrame( 0 )->Height * scale) / 2), scale, scale, 0 );

	int xPos = (DISPLAY->GetWidth() / 2) - (bootBarSize / 2);
	int yPos = DISPLAY->GetHeight() - 12;
	al_draw_filled_rectangle( xPos, yPos, xPos + bootBarSize, yPos + 8, al_map_rgb( 255, 128, 0 ) );
}

void BootUp::StartGame()
{
	delete Framework::System->ProgramStages->Pop();
	// Framework::System->ProgramStages->Push( new MainMenu() );
}

bool BootUp::IsTransition()
{
	return false;
}
