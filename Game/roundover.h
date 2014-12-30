
#pragma once

#include "includes.h"
#include "arena.h"

#define ROUNDOVER_STEPSINOUT		20
#define ROUNDOVER_STAGETIME			60

class RoundOver : public Stage
{
	private:
		int winner;
		float overbanner[8];
		bool leaving;
		int overbannerXv;

		std::string textDisplay;
		int textWidth;
		int textXposition;
		int textXv;

		ALLEGRO_FONT* fntTitle;
		int fntTitleHeight;

		int stagetime;

		Arena* currentArena;
		

  public:
		RoundOver(int PlayerWhoWon);

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
