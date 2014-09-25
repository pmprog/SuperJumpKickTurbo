
#pragma once

#include "../includes.h"
#include "../Primitives/vector2.h"
#include "../event.h"
#include "../framework.h"

#ifndef Form
class Form;
#endif

class Control
{
	private:
		ALLEGRO_BITMAP* controlArea;

		void PreRender();
		void PostRender();
		

	protected:
		Control* owningControl;
		Control* focusedChild;
		bool mouseInside;
		bool mouseDepressed;
		Vector2 resolvedLocation;

		virtual void OnRender();

		void ResolveLocation();

		Control* GetRootControl();

	public:
		std::string Name;
		Vector2 Location;
		Vector2 Size;
		ALLEGRO_COLOR BackgroundColour;
		ALLEGRO_COLOR ForegroundColour;
		bool CanFocus;
		bool AllowTab;

		std::vector<Control*> Controls;

		Control(Control* Owner);
		virtual ~Control();

		void Focus();
		bool IsFocused();

		virtual void EventOccured(Event* e);
		void Render();
		virtual void Update();
		virtual void UnloadResources();

		Control* operator[]( int Index );
		Control* FindControl( std::string ID );

		Control* GetParent();
		Form* GetForm();
};

