
#pragma once

#include "control.h"

#define VSCROLLBAR_SELECT_TOGGLE_TIME		30

class VScrollBar : public Control
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

		VScrollBar( Control* Owner );
		virtual ~VScrollBar();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};