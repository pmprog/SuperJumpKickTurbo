
#pragma once

#include "includes.h"
#include "fighter.h"

class NetworkMenu : public Stage
{
	private:
		Fighter::FighterController localController;
		Form* netForm;
		TTFFont* fontForm;
		TextEdit* te;

	public:

		NetworkMenu(Fighter::FighterController Controller);
		~NetworkMenu();

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
