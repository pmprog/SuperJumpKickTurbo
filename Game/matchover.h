
#pragma once

#include "includes.h"
#include "arena.h"

class MatchOver : public Stage
{
	private:
		Arena* gameArena;

		int winner;
		float overbannerL[8];
		float overbannerR[8];
		float bannerspeed;
		int textstart;

		ALLEGRO_FONT* fntStatus;
		ALLEGRO_FONT* fntMenu;

		void DrawDropShadowText( ALLEGRO_FONT* Font, ALLEGRO_COLOR Colour, float X, float Y, int Flags, const char* Text );

  public:
		MatchOver(int WinnerIs);

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
