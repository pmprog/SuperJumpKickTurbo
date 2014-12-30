
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"

#include "fighter.h"

class Menu : public Stage
{
	private:
		ALLEGRO_BITMAP* imgSuper;
		ALLEGRO_BITMAP* imgJumpKick;
		ALLEGRO_BITMAP* imgBackground;
		SpriteSheet* imgTurbo;

		ALLEGRO_FONT* fntTitle;

		int menuTime;
		int menuSelection;

		ALLEGRO_COLOR menuSelectedColour;
		ALLEGRO_COLOR menuItemColour;

		Fighter* TitleFighters[3];

		int backgroundX;

  public:

		static int Player1Joystick;
		static int Player2Joystick;

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void EventOccurred(Event *e);
    virtual void Update();
    virtual void Render();
		virtual bool IsTransition();
};
