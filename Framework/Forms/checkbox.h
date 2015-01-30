
#pragma once

#include "control.h"

#define CHECKBOX_SELECT_TOGGLE_TIME		30

class CheckBox : public Control
{

	private:
		bool selectedDraw;
		int selectedTimer;

		ALLEGRO_BITMAP* imagechecked;
		ALLEGRO_BITMAP* imageunchecked;

		void LoadResources();

	protected:
		virtual void OnRender();

	public:
		bool Checked;

		CheckBox( Control* Owner );
		virtual ~CheckBox();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};
