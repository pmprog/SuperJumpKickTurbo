
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"

class SettingsMenu : public Stage
{
	private:
		Form* uiForm;

		ALLEGRO_FONT* fontTitle;
		int fontHeight;

		ALLEGRO_FONT* fontCredits;
		int fontCreditsHeight;

		TTFFont* fontUI;

		int menuSelection;

		ALLEGRO_COLOR menuSelectedColour;
		ALLEGRO_COLOR menuItemColour;

		int DrawMenuItem( int MenuID, int CurrentY, std::string Text );

		void CreateVideoForm();

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
