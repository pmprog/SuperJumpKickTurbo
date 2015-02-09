
#pragma once

#include "includes.h"
#include "fighter.h"

#define PACKET_TYPE_PING	0
#define PACKET_TYPE_INPUT	1
#define PACKET_TYPE_DISCONNECT	-1

typedef struct GamePacketInput
{
	bool JumpPressed;
	bool KickPressed;
	int X;
	int Y;
} GamePacketInput;

typedef struct GamePacket
{
	uint32_t Type;
	uint64_t FrameCount;
	GamePacketInput Input;
} GamePacket;

