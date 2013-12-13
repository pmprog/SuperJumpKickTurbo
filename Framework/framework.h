
#pragma once

#include "graphicslib.h"
#include "event.h"

#include "../Library/configfile.h"
#include "stagestack.h"

#define FRAMES_PER_SECOND 100
#define FRAME_TIME_IN_MS  1000 / FRAMES_PER_SECOND

class Framework
{
  private:
    bool quitProgram;
    int deltaTime;
    int framesToProcess;

    SDL_Surface* displaySurface;
		std::list<Event*> eventQueue;
		SDL_mutex* eventMutex;

  public:
    static Framework* System;

    Framework();
    ~Framework();

    void Run();
		void ShutdownFramework();
		bool IsShuttingDown() { return quitProgram; };

		void ProcessEvents();
    void PushEvent( Event* e );
		void PushSDLEvent( SDL_Event* e );

		void ProcessUpdates( int Delta );

    ConfigFile* Settings;
    StageStack* ProgramStages;

    void InitialiseDisplay();
    void ShutdownDisplay();
    int GetDisplayWidth();
    int GetDisplayHeight();
		void SetWindowTitle( std::string* NewTitle );

    void InitialiseAudioSystem();
    void ShutdownAudioSystem();
    void PlayMusic( std::string Filename, bool Loop );
    void StopMusic();

    void SaveSettings();

};

// Unfortunately Sparrow doesn't really support C++
void resizeWindow( Uint16 w, Uint16 h );
int engineUpdate(Uint32 steps);
void engineDraw();
void pushSDLEvent( SDL_Event* e );