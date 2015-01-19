
#pragma once

#include "includes.h"
#include "../Framework/Display/spritesheet.h"
#include "fighter.h"
#include "networkpacket.h"

#define SLOWMODE_DELAY_FRAMES		2
#define ROUND_TIME							20

class Arena : public Stage
{
	private:
		SpriteSheet* CollisionGraphics;
		Animation* CollisionAnimation;
		Vector2* CollisionsAt[2];

		ALLEGRO_FONT* fntTimer;
		int CountdownTimer;
		int CountdownTimerTicker;
		bool DisableTimer;

		SpriteSheet* RoundMarkers;
		ALLEGRO_BITMAP* Background;
		Vector2 Camera;

		int CamXMove;
		int CamYMove;

		int SlowMode;
		int SlowModeDelay;

		bool DebugReverse;

		GamePacket CurrentSync;
		GamePacket LastGoodSync;

		uint64_t ClockRoundFrameCount[ROUND_TIME];

		void FixCameraPosition();
		void TickRoundClock();

		Fighter* GetPlayerWithControls( Fighter::FighterController Controller );

		void SendSyncPacket();
		void CheckSyncPacket( GamePacket* Packet );

  public:

		static int Player1TotalWins;
		static int Player2TotalWins;

		bool DemoMode;
		uint64_t RoundFrameCount;

		Fighter* Player1;
		int Player1Wins;
		Fighter* Player2;
		int Player2Wins;

		int ArenaWidth;

		Arena();
		Arena( std::string LocationImage, Fighter* P1, Fighter* P2 );
		~Arena();

		void ResetArena();
		Fighter* GetOpponent(Fighter* Current);

		bool State_Load(uint64_t FrameCount);
		void AddCollisionAt( Vector2* Location );

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
