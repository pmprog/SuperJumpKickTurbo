
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"
#include "fighter.h"

class PlayerSelect : public Stage
{
	private:

		bool p1active;
		Fighter* p1char;
		bool p2active;
		Fighter* p2char;

		Arena* gameArena;

  public:

		PlayerSelect( bool Player1Active, bool Player2Active );

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
