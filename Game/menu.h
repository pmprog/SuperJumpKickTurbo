
#pragma once

#include "../Framework/stage.h"
#include "../Framework/graphicslib.h"
#include "fighter_akuma.h"

class Menu : public Stage
{
	private:
		Akuma*	titleChar;
		SDL_Surface* titleSuper;
		SDL_Surface* titleJumpFighter;

		spFont* titleFont;
		spFont* titleFontSel;

		float ScreenScale;
		int menuTimer;
		int menuOption;
		int turboValue;

  public:
    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void EventOccurred(Event *e);
    virtual void Update();
    virtual void Render();
};