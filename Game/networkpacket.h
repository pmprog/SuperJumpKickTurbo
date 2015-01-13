
#pragma once

#include "includes.h"

#define PACKET_TYPE_PING	0
#define PACKET_TYPE_INPUT	1
#define PACKET_TYPE_CHECK	2
#define PACKET_TYPE_DISCONNECT	-1

typedef struct GamePacketInput
{
	bool JumpPressed;
	bool KickPressed;
} GamePacketInput;

typedef struct GamePacketSyncCheck
{
	int Player1X;
	int Player1Y;
	int Player1State;
	int Player2X;
	int Player2Y;
	int Player2State;
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

