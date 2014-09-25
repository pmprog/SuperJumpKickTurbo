
#pragma once

#include "control.h"
#include "../Display/fontcache.h"

class Label : public Control
{

	private:
		std::string text;
		TTFFont* font;

	protected:
		virtual void OnRender();

	public:
		HorizontalAlignment TextHAlign;
		VerticalAlignment TextVAlign;

		Label(Control* Owner, std::string Text, TTFFont* Font);
		virtual ~Label();

		virtual void EventOccured(Event* e);
		virtual void Update();
		virtual void UnloadResources();

		std::string GetText();
		void SetText( std::string Text );
};

