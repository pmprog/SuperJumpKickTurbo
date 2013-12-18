
#pragma once

#include "../Framework/stage.h"
#include "../Framework/graphicslib.h"
#include "../Library/animation.h"
#include "fighter.h"

class Fight : public Stage
{
	private:
		float ScreenScale;

		SDL_Surface* Background;

		int ArenaWidth;
		int ArenaHeight;
		int CameraX;
		int CameraY;

		Fighter* PlayerA;
		Fighter* PlayerB;

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