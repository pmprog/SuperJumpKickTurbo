
#pragma once

#include "../includes.h"
#include "../Primitives/vector2.h"

#define DISPLAY	Display::CurrentDisplay

struct DisplayScaleMode
{
	enum ScaleMode
	{
		Letterbox,
		Fill
	};
};

class Display
{

	private:
    ALLEGRO_DISPLAY_MODE screenMode;
		ALLEGRO_DISPLAY* screen;
		ALLEGRO_BITMAP* screenGameBuffer;
		ALLEGRO_BITMAP* screenRetarget;

		Vector2 gameSize;
		Vector2 screenSize;

		Vector2 gameScreenLocation;
		Vector2 gameScreenSize;

	public:
		static Display* CurrentDisplay;

		Display( int GameWidth, int GameHeight );
		~Display();

    void Initialise( int ScreenWidth, int ScreenHeight, bool Fullscreen, DisplayScaleMode::ScaleMode Scale );
    void Shutdown();

		ALLEGRO_EVENT_SOURCE* GetEventSource();

    int GetWidth();
    int GetHeight();
		void SetTitle( std::string* NewTitle );
		void SetTitle( std::string NewTitle );

		ALLEGRO_BITMAP* GetCurrentTarget();
		void ClearTarget();
		void SetTarget( ALLEGRO_BITMAP* Target );

		void Render();

		void MouseVisible( bool Visible );
		Vector2* ScreenPointToGamePoint( Vector2* Point );
		float ScreenXToGameX( float X );
		float ScreenYToGameY( float Y );

};