
#pragma once

#include "includes.h"
#include "configfile.h"
#include "Display/display.h"
#include "Sound/audio.h"
#include "event.h"

#include "Stages/stagestack.h"
#include "Display/bitmapcache.h"
#include "Display/fontcache.h"

#ifdef NETWORK_SUPPORT
#include "Network/network.h"
#endif

#ifdef DOWNLOAD_SUPPORT
#include "Network/download.h"
#endif

#define FRAMEWORK	Framework::System

class Framework
{
  private:
    bool quitProgram;

		int framesPerSecond;
		ALLEGRO_TIMER* frameTimer;
    int framesToProcess;
		bool enableSlowDown;


		ALLEGRO_EVENT_QUEUE* eventAllegro;
		std::list<Event*> eventQueue;
		ALLEGRO_MUTEX* eventMutex;

		std::vector<ALLEGRO_JOYSTICK*> joystickIDs;
		
		void GetJoystickIDs();

  public:
    static Framework* System;

    ConfigFile* Settings;
    StageStack* ProgramStages;

    Framework( int Width, int Height, int Framerate, bool DropFrames );
    ~Framework();

    void Run();
		void ProcessEvents();
    void PushEvent( Event* e );
		void TranslateAllegroEvents();
		void ShutdownFramework();
		bool IsShuttingDown() { return quitProgram; };

    void SaveSettings();

		bool IsSlowMode();
		void SetSlowMode(bool SlowEnabled);
		int GetFramesPerSecond();

		void RegisterEventSource( ALLEGRO_EVENT_SOURCE* Source );
		void UnregisterEventSource( ALLEGRO_EVENT_SOURCE* Source );
};
