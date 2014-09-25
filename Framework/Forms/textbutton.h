
#pragma once

#include "control.h"
#include "../Display/fontcache.h"

#define TEXTBUTTON_SELECT_TOGGLE_TIME		30

class TextButton : public Control
{

	private:
		bool selectedDraw;
		int selectedTimer;
		std::string text;
		TTFFont* font;

	protected:
		virtual void OnRender();

	public:
		HorizontalAlignment TextHAlign;
		VerticalAlignment TextVAlign;

		TextButton(Control* Owner, std::string Text, TTFFont* Font);
		virtual ~TextButton();

		virtual void EventOccured(Event* e);
		virtual void Update();
		virtual void UnloadResources();

		std::string GetText();
		void SetText( std::string Text );
};

