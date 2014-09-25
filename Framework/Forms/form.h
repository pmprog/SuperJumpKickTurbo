
#pragma once

#include "control.h"

class Form : public Control
{
	private:
		Control* activeControl;

		std::vector<Control*> focusableControls;

		void GenerateFocusList(Control* Parent);

	protected:
		virtual void OnRender();

	public:
		Form();
		virtual ~Form();

		void SetFocus(Control* ActiveControl);
		void FocusPrevious();
		void FocusNext();
		Control* GetActiveControl();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};

