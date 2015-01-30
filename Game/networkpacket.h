
#pragma once

#include "includes.h"
#include "fighter.h"

#define PACKET_TYPE_PING	0
#define PACKET_TYPE_INPUT	1
#define PACKET_TYPE_STATE	2
#define PACKET_TYPE_CHECK	3
#define PACKET_TYPE_DISCONNECT	-1

typedef struct GamePacketInput
{
	bool JumpPressed;
	bool KickPressed;
	int X;
	int Y;
	Fighter::FighterStates State;
} GamePacketInput;

typedef struct GamePacketPlayerSync
{
	int X;
	int Y;
	Fighter::FighterStates State;
	int StateTime;
	bool FighterHit;
	bool FacingLeft;
} GamePacketPlayerSync;

typedef struct GamePacketSyncCheck
{
	int CountdownTimer;
	int CountdownTimerTicker;
	GamePacketPlayerSync Player1;
	GamePacketPlayerSync Player2;
} GamePacketSyncCheck;

typedef union GamePacketData
{
	GamePacketInput Input;
	GamePacketSyncCheck Sync;
	Fighter::FighterSaveState State;
} GamePacketData;

typedef struct GamePacket
{
	uint32_t Type;
	uint64_t FrameCount;
	GamePacketData Data;
} GamePacket;

