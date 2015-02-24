
#pragma once

#include "includes.h"

class SettingsKeySet : public Stage
{
	private:
		std::string keyname;
		TextButton* sourcebutton;
		ALLEGRO_FONT* fontTitle;
		int fontHeight;

  public:

		SettingsKeySet( std::string KeyName, TextButton* Source );

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
