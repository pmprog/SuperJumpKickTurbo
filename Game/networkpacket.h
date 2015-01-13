
#pragma once

#include "includes.h"
#include "fighter.h"

#define PACKET_TYPE_PING	0
#define PACKET_TYPE_INPUT	1
#define PACKET_TYPE_CHECK	2
#define PACKET_TYPE_DISCONNECT	-1

typedef struct GamePacketInput
{
	bool JumpPressed;
	bool KickPressed;
	int X;
	int Y;
	Fighter::FighterStates State;
} GamePacketInput;

typedef struct GamePacketSyncCheck
{
	int Player1X;
	int Player1Y;
	Fighter::FighterStates Player1State;
	int Player2X;
	int Player2Y;
	Fighter::FighterStates Player2State;
} GamePacketSyncCheck;

typedef union GamePacketData
{
	GamePacketInput Input;
	GamePacketSyncCheck Sync;
} GamePacketData;

typedef struct GamePacket
{
	uint32_t Type;
	uint64_t FrameCount;
	GamePacketData Data;
} GamePacket;

