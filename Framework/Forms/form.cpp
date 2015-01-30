#include "form.h"

Form::Form() : Control( nullptr ), activeControl( nullptr )
{
}

Form::~Form()
{
}

void Form::EventOccured( Event* e )
{
	Control::EventOccured( e );
}

void Form::OnRender()
{
}

void Form::Update()
{
	Control::Update();
	ResolveLocation();
}

void Form::UnloadResources()
{
	Control::UnloadResources();
}

void Form::SetFocus(Control* ActiveControl)
{
	Event* e;

	//if( activeControl == ActiveControl )
	//{
	//	return;
	//}

	if( activeControl != nullptr )
	{
		e = new Event();
		e->Type = EVENT_FORM_INTERACTION;
		e->Data.Forms.RaisedBy = activeControl;
		e->Data.Forms.EventFlag = FormEventType::LostFocus;
		memset( (void*)&e->Data.Forms.KeyInfo, 0, sizeof(FRAMEWORK_KEYBOARD_EVENT) );
		memset( (void*)&e->Data.Forms.MouseInfo, 0, sizeof(FRAMEWORK_MOUSE_EVENT) );
		e->Data.Forms.AdditionalData = nullptr;
		FRAMEWORK->PushEvent( e );
	}
	activeControl = ActiveControl;
	if( activeControl != nullptr )
	{
		e = new Event();
		e->Type = EVENT_FORM_INTERACTION;
		e->Data.Forms.RaisedBy = activeControl;
		e->Data.Forms.EventFlag = FormEventType::GotFocus;
		memset( (void*)&e->Data.Forms.KeyInfo, 0, sizeof(FRAMEWORK_KEYBOARD_EVENT) );
		memset( (void*)&e->Data.Forms.MouseInfo, 0, sizeof(FRAMEWORK_MOUSE_EVENT) );
		e->Data.Forms.AdditionalData = nullptr;
		FRAMEWORK->PushEvent( e );
	}
}

void Form::GenerateFocusList( Control* Parent )
{
	if( Parent == this )
	{
		focusableControls.clear();
	}

	for( auto c = Parent->Controls.begin(); c != Parent->Controls.end(); c++ )
	{
		Control* ctrl = (Control*)*c;
		if( ctrl->CanFocus )
		{
			focusableControls.push_back( ctrl );
		}
		if( ctrl->Controls.size() > 0 )
		{
			GenerateFocusList( ctrl );
		}
	}
}

void Form::FocusPrevious()
{
	Control* prevctrl = nullptr;
	GenerateFocusList( this );
	for( auto c = focusableControls.begin(); c != focusableControls.end(); c++ )
	{
		Control* ctrl = (Control*)*c;
		if( ctrl == activeControl )
		{
			break;
		} else {
			prevctrl = ctrl;
		}
	}
	if( prevctrl == nullptr )
	{
		prevctrl = focusableControls.front();
	}
	SetFocus( prevctrl );
}

void Form::FocusNext()
{
	Control* prevctrl = nullptr;
	GenerateFocusList( this );
	for( auto c = focusableControls.rbegin(); c != focusableControls.rend(); c++ )
	{
		Control* ctrl = (Control*)*c;
		if( ctrl == activeControl )
		{
			break;
		} else {
			prevctrl = ctrl;
		}
	}
	if( prevctrl == nullptr )
	{
		prevctrl = focusableControls.back();
	}
	SetFocus( prevctrl );
}

Control* Form::GetActiveControl()
{
	return activeControl;
}
