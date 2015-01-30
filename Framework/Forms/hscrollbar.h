
#pragma once

#include "control.h"

#define HSCROLLBAR_SELECT_TOGGLE_TIME		30

class HScrollBar : public Control
{
	private:
		bool selectedDraw;
		int selectedTimer;
		bool capture;

		void LoadResources();

	protected:
		virtual void OnRender();

	public:
		int Minimum;
		int Maximum;
		int Value;
		int LargeChange;

		HScrollBar( Control* Owner );
		virtual ~HScrollBar();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};