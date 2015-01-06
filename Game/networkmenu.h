
#pragma once

#include "includes.h"
#include "fighter.h"

class NetworkMenu : public Stage
{
	private:
		Fighter::FighterController localController;

	public:

		NetworkMenu(Fighter::FighterController Controller);

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
