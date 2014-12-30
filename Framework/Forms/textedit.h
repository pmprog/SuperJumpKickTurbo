
#pragma once

#include "control.h"
#include "../Display/fontcache.h"

#define TEXTEDITOR_CARET_TOGGLE_TIME		30

class TextEdit : public Control
{

	private:
		bool caretDraw;
		int caretTimer;
		std::string text;
		TTFFont* font;
		bool editting;
		bool editShift;
		bool editAltGr;

		void RaiseEvent( FormEventType Type );

	protected:
		virtual void OnRender();

	public:
		int SelectionStart;
		HorizontalAlignment TextHAlign;
		VerticalAlignment TextVAlign;

		TextEdit(Control* Owner, std::string Text, TTFFont* Font);
		virtual ~TextEdit();

		virtual void EventOccured(Event* e);
		virtual void Update();
		virtual void UnloadResources();

		std::string GetText();
		void SetText( std::string Text );

		void BeginEdit();
};

