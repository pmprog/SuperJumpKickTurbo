
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"

class SettingsMenu : public Stage
{
	private:
		ALLEGRO_FONT* fontTitle;
		int fontHeight;

		int menuSelection;

		ALLEGRO_COLOR menuSelectedColour;
		ALLEGRO_COLOR menuItemColour;

		int DrawMenuItem( int MenuID, int CurrentY, std::string Text );

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
