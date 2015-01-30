
#pragma once

#include "includes.h"
#include "arena.h"

class MatchOver : public Stage
{
	private:
		Arena* gameArena;

		float overbannerL[8];
		float overbannerR[8];
		float bannerspeed;


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
