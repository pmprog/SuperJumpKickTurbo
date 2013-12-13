
#pragma once

#include "event.h"

#ifndef Framework
class Framework;
#endif // Framework

class Stage
{
  public:
    // Stage control
    virtual void Begin() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Finish() = 0;

    virtual void EventOccurred(Event *e) = 0;

    virtual void Update() = 0;
    virtual void Render() = 0;

};
