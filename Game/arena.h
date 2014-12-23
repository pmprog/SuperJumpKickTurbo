
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"
#include "fighter.h"

#define SLOWMODE_DELAY_FRAMES		2

class Arena : public Stage
{
	private:
		ALLEGRO_FONT* fntTimer;
		int CountdownTimer;
		int CountdownTimerTicker;
		bool DisableTimer;

		Fighter* Player1;
		int Player1Wins;
		Fighter* Player2;
		int Player2Wins;
		ALLEGRO_BITMAP* Background;
		Vector2 Camera;

		int CamXMove;
		int CamYMove;
		int arenaWidth;

		int SlowMode;
		int SlowModeDelay;

  public:
		long RoundFrameCount;

		Arena();
		Arena( std::string LocationImage, Fighter* P1, Fighter* P2 );
		~Arena();

		void ResetArena();
		Fighter* GetOpponent(Fighter* Current);

		bool State_Load(long FrameCount);

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
