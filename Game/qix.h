
#pragma once

#include "../Framework/stage.h"
#include "../Framework/graphicslib.h"

class Qix : public Stage
{
	private:

  public:
    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void EventOccurred(Event *e);
    virtual void Update();
    virtual void Render();
};