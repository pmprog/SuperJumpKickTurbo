
#pragma once

#include "includes.h"
#include "arena.h"

class RoundCountIn : public Stage
{
	private:
		float overbanner[8];
		float bannerspeed;
		int countdown;
		ALLEGRO_FONT* fntTitle;
		int fntTitleHeight;


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
