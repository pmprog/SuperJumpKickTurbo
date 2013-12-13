
#pragma once

#include "graphicslib.h"
#include "../Library/memory.h"
#include <string>

enum EventTypes
{
  EVENT_WINDOW_ACTIVATE,
  EVENT_WINDOW_DEACTIVATE,
	EVENT_WINDOW_RESIZE,
	EVENT_WINDOW_MANAGER,
	EVENT_WINDOW_CLOSED,
  EVENT_KEY_DOWN,
  EVENT_KEY_UP,
  EVENT_MOUSE_DOWN,
  EVENT_MOUSE_UP,
  EVENT_MOUSE_MOVE,
  EVENT_JOYSTICK_AXIS,
	EVENT_JOYSTICK_HAT,
	EVENT_JOYSTICK_BALL,
  EVENT_JOYSTICK_BUTTON_DOWN,
  EVENT_JOYSTICK_BUTTON_UP,
  EVENT_TIMER_TICK,
  EVENT_NETWORK_CONNECTION_REQUEST,
  EVENT_NETWORK_PACKET_RECEIVED,
  EVENT_NETWORK_DISCONNECTED,
  EVENT_DOWNLOAD_PROGRESS,
  EVENT_DOWNLOAD_COMPLETE,
  EVENT_AUDIO_FINISHED,
  EVENT_UNDEFINED
};

typedef struct FRAMEWORK_DOWNLOAD_EVENT
{
  std::string* URL;
  Memory* Contents;
  double DownloadedBytes;
  double TotalBytesToDownload;
  double UploadedBytes;
  double TotalBytesToUpload;
} FRAMEWORK_DOWNLOAD_EVENT;

typedef struct FRAMEWORK_NETWORK_EVENT
{
  int Reserved;
} FRAMEWORK_NETWORK_EVENT;

typedef struct FRAMEWORK_DISPLAY_EVENT
{
	SDL_ActiveEvent Active;
	SDL_ResizeEvent Resize;
	SDL_SysWMEvent WindowManager;
} FRAMEWORK_DISPLAY_EVENT;

typedef struct FRAMEWORK_JOYSTICK_EVENT
{
	SDL_JoyAxisEvent Axis;
	SDL_JoyBallEvent Ball;
	SDL_JoyHatEvent Hat;
	SDL_JoyButtonEvent Button;
} FRAMEWORK_JOYSTICK_EVENT;

typedef struct FRAMEWORK_MOUSE_EVENT
{
	SDL_MouseMotionEvent Motion;
	SDL_MouseButtonEvent Button;
} FRAMEWORK_MOUSE_EVENT;

typedef union EventData
{
	FRAMEWORK_DISPLAY_EVENT		Display;
	FRAMEWORK_JOYSTICK_EVENT	Joystick;
	SDL_KeyboardEvent					Keyboard;
	FRAMEWORK_MOUSE_EVENT			Mouse;
	SDL_UserEvent							User;
  FRAMEWORK_NETWORK_EVENT   Network;
  FRAMEWORK_DOWNLOAD_EVENT  Download;
} EventData;


class Event
{
  public:
    EventTypes Type;
    EventData Data;

    Event();
		Event( SDL_Event* Source );
};
