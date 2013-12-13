
#include "event.h"

Event::Event()
{
  Type = EVENT_UNDEFINED;
}

Event::Event( SDL_Event* Source )
{
	switch( Source->type )
	{
		case SDL_MOUSEBUTTONDOWN:
			Type = EVENT_MOUSE_DOWN;
			memcpy( (void*)&Data.Mouse.Button, (void*)&Source->button, sizeof(Source->button) );
			break;
		case SDL_MOUSEBUTTONUP:
			Type = EVENT_MOUSE_UP;
			memcpy( (void*)&Data.Mouse.Button, (void*)&Source->button, sizeof(Source->button) );
			break;
		case SDL_MOUSEMOTION:
			Type = EVENT_MOUSE_MOVE;
			memcpy( (void*)&Data.Mouse.Motion, (void*)&Source->motion, sizeof(Source->motion) );
			break;
		case SDL_KEYDOWN:
			Type = EVENT_KEY_DOWN;
			memcpy( (void*)&Data.Keyboard, (void*)&Source->key, sizeof(Source->key) );
			break;
		case SDL_KEYUP:
			Type = EVENT_KEY_UP;
			memcpy( (void*)&Data.Keyboard, (void*)&Source->key, sizeof(Source->key) );
			break;

		case SDL_JOYAXISMOTION:
			Type = EVENT_JOYSTICK_AXIS;
			memcpy( (void*)&Data.Joystick.Axis, (void*)&Source->jaxis, sizeof(Source->jaxis) );
			break;
		case SDL_JOYBALLMOTION:
			Type = EVENT_JOYSTICK_BALL;
			memcpy( (void*)&Data.Joystick.Ball, (void*)&Source->jball, sizeof(Source->jball) );
			break;
		case SDL_JOYHATMOTION:
			Type = EVENT_JOYSTICK_HAT;
			memcpy( (void*)&Data.Joystick.Hat, (void*)&Source->jhat, sizeof(Source->jhat) );
			break;
		case SDL_JOYBUTTONDOWN:
			Type = EVENT_JOYSTICK_BUTTON_DOWN;
			memcpy( (void*)&Data.Joystick.Button, (void*)&Source->jbutton, sizeof(Source->jbutton) );
			break;
		case SDL_JOYBUTTONUP:
			Type = EVENT_JOYSTICK_BUTTON_UP;
			memcpy( (void*)&Data.Joystick.Button, (void*)&Source->jbutton, sizeof(Source->jbutton) );
			break;

		case SDL_ACTIVEEVENT:
			Type = EVENT_UNDEFINED;
			if( Source->active.state == SDL_APPINPUTFOCUS || Source->active.state == SDL_APPACTIVE )
			{
				if( Source->active.gain != 0 )
				{
					Type = EVENT_WINDOW_ACTIVATE;
				} else {
					Type = EVENT_WINDOW_DEACTIVATE;
				}
				memcpy( (void*)&Data.Display.Active, (void*)&Source->active, sizeof(Source->active) );
			}
			break;
		case SDL_VIDEORESIZE:
			Type = EVENT_WINDOW_RESIZE;
			memcpy( (void*)&Data.Display.Resize, (void*)&Source->resize, sizeof(Source->resize) );
			break;
		case SDL_SYSWMEVENT:
			Type = EVENT_WINDOW_MANAGER;
			memcpy( (void*)&Data.Display.WindowManager, (void*)&Source->syswm, sizeof(Source->syswm) );
			break;

		case SDL_QUIT:
			Type = EVENT_WINDOW_CLOSED;
			break;

		default:
			Type = EVENT_UNDEFINED;
			break;
	}
	
}
