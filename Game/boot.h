
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"

class BootUp : public Stage
{
	private:
		int bootBarAdjust;
		int bootBarSize;
		SpriteSheet* logoSprite;
		int logoFadeIn;

		Angle* a;

		void StartGame();

  public:
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
