
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"
#include "fighter.h"

class Arena : public Stage
{
	private:
		Fighter* Player1;
		Fighter* Player2;
		ALLEGRO_BITMAP* Background;
		Vector2 Camera;

		int CamXMove;
		int CamYMove;

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
