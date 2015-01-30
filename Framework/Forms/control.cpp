
#include "control.h"
#include "forms.h"

Control::Control(Control* Owner) : Name("Control"), owningControl(Owner), focusedChild(nullptr), BackgroundColour(al_map_rgb( 80, 80, 80 )), ForegroundColour(al_map_rgb(255, 255, 255)), CanFocus(false), AllowTab(false), mouseInside(false), mouseDepressed(false), controlArea(nullptr)
{
	if( Owner != nullptr )
	{
		Owner->Controls.push_back( this );
	}
}

Control::~Control()
{
	UnloadResources();
	// Delete controls
	while( Controls.size() > 0 )
	{
		Control* c = Controls.back();
		Controls.pop_back();
		delete c;
	}
}

void Control::Focus()
{
	Form* f = GetForm();
	if( f != nullptr && this->CanFocus )
	{
		f->SetFocus( this );
	}
}

bool Control::IsFocused()
{
	Form* f = GetForm();
	if( f != nullptr )
	{
		return (f->GetActiveControl() == this);
	}
	return false;
}

void Control::ResolveLocation()
{
	if( owningControl == nullptr )
	{
		resolvedLocation.X = Location.X;
		resolvedLocation.Y = Location.Y;
	} else {
		resolvedLocation.X = owningControl->resolvedLocation.X + Location.X;
		resolvedLocation.Y = owningControl->resolvedLocation.Y + Location.Y;
	}

	for( auto ctrlidx = Controls.rbegin(); ctrlidx != Controls.rend(); ctrlidx++ )
	{
		Control* c = (Control*)*ctrlidx;
		c->ResolveLocation();
	}
}

void Control::EventOccured( Event* e )
{
	for( auto ctrlidx = Controls.rbegin(); ctrlidx != Controls.rend(); ctrlidx++ )
	{
		Control* c = (Control*)*ctrlidx;
		c->EventOccured( e );
		if( e->Handled )
		{
			return;
		}
	}

	Event* newevent;

	if( e->Type == EVENT_MOUSE_MOVE )
	{
		if( e->Data.Mouse.X >= resolvedLocation.X && e->Data.Mouse.X < resolvedLocation.X + Size.X && e->Data.Mouse.Y >= resolvedLocation.Y && e->Data.Mouse.Y < resolvedLocation.Y + Size.Y )
		{
			if( !mouseInside )
			{
				newevent = new Event();
				newevent->Type = EVENT_FORM_INTERACTION;
				newevent->Data.Forms.RaisedBy = this;
				newevent->Data.Forms.EventFlag = FormEventType::MouseEnter;
				memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
				newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
				newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
				memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
				newevent->Data.Forms.AdditionalData = nullptr;
				FRAMEWORK->PushEvent( newevent );
				mouseInside = true;
			}

			newevent = new Event();
			newevent->Type = EVENT_FORM_INTERACTION;
			newevent->Data.Forms.RaisedBy = this;
			newevent->Data.Forms.EventFlag = FormEventType::MouseMove;
			memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
			newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
			newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
			memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
			newevent->Data.Forms.AdditionalData = nullptr;
			FRAMEWORK->PushEvent( newevent );

			e->Handled = true;
		} else {
			if( mouseInside )
			{
				newevent = new Event();
				newevent->Type = EVENT_FORM_INTERACTION;
				newevent->Data.Forms.RaisedBy = this;
				newevent->Data.Forms.EventFlag = FormEventType::MouseLeave;
				memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
				newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
				newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
				memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
				newevent->Data.Forms.AdditionalData = nullptr;
				FRAMEWORK->PushEvent( newevent );
				mouseInside = false;
			}
		}
	}

	if( e->Type == EVENT_MOUSE_DOWN )
	{
		if( mouseInside )
		{
			newevent = new Event();
			newevent->Type = EVENT_FORM_INTERACTION;
			newevent->Data.Forms.RaisedBy = this;
			newevent->Data.Forms.EventFlag = FormEventType::MouseDown;
			memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
			newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
			newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
			memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
			newevent->Data.Forms.AdditionalData = nullptr;
			FRAMEWORK->PushEvent( newevent );
			mouseDepressed = true;

			e->Handled = true;
		}
	}

	if( e->Type == EVENT_MOUSE_UP )
	{
		if( mouseInside )
		{
			newevent = new Event();
			newevent->Type = EVENT_FORM_INTERACTION;
			newevent->Data.Forms.RaisedBy = this;
			newevent->Data.Forms.EventFlag = FormEventType::MouseUp;
			memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
			newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
			newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
			memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
			newevent->Data.Forms.AdditionalData = nullptr;
			FRAMEWORK->PushEvent( newevent );

			if( mouseDepressed )
			{
				newevent = new Event();
				newevent->Type = EVENT_FORM_INTERACTION;
				newevent->Data.Forms.RaisedBy = this;
				newevent->Data.Forms.EventFlag = FormEventType::MouseClick;
				memcpy( (void*)&newevent->Data.Forms.MouseInfo, (void*)&e->Data.Mouse, sizeof( FRAMEWORK_MOUSE_EVENT ) );
				newevent->Data.Forms.MouseInfo.X -= resolvedLocation.X;
				newevent->Data.Forms.MouseInfo.Y -= resolvedLocation.Y;
				memset( (void*)&newevent->Data.Forms.KeyInfo, 0, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
				newevent->Data.Forms.AdditionalData = nullptr;
				FRAMEWORK->PushEvent( newevent );
			}

			e->Handled = true;
		}
		mouseDepressed = false;
	}

	if( e->Type == EVENT_KEY_DOWN || e->Type == EVENT_KEY_UP )
	{
		if( IsFocused() )
		{
			newevent = new Event();
			newevent->Type = EVENT_FORM_INTERACTION;
			newevent->Data.Forms.RaisedBy = this;
			newevent->Data.Forms.EventFlag = (e->Type == EVENT_KEY_DOWN ? FormEventType::KeyDown : FormEventType::KeyUp);
			memcpy( (void*)&newevent->Data.Forms.KeyInfo, (void*)&e->Data.Keyboard, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
			memset( (void*)&newevent->Data.Forms.MouseInfo, 0, sizeof( FRAMEWORK_MOUSE_EVENT ) );
			newevent->Data.Forms.AdditionalData = nullptr;
			FRAMEWORK->PushEvent( newevent );

			e->Handled = true;
		}
	}
	if( e->Type == EVENT_KEY_PRESS )
	{
		if( IsFocused() )
		{
			if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_TAB && !AllowTab )
			{
				if( (e->Data.Keyboard.Modifiers & ALLEGRO_KEYMOD_SHIFT) == ALLEGRO_KEYMOD_SHIFT )
				{
					this->GetForm()->FocusPrevious();
					e->Handled = true;
					return;
				} else {
					this->GetForm()->FocusNext();
					e->Handled = true;
					return;
				}
			}

			newevent = new Event();
			newevent->Type = EVENT_FORM_INTERACTION;
			newevent->Data.Forms.RaisedBy = this;
			newevent->Data.Forms.EventFlag = FormEventType::KeyPress;
			memcpy( (void*)&newevent->Data.Forms.KeyInfo, (void*)&e->Data.Keyboard, sizeof( FRAMEWORK_KEYBOARD_EVENT ) );
			memset( (void*)&newevent->Data.Forms.MouseInfo, 0, sizeof( FRAMEWORK_MOUSE_EVENT ) );
			newevent->Data.Forms.AdditionalData = nullptr;
			FRAMEWORK->PushEvent( newevent );

			e->Handled = true;
		}
	}
}

void Control::Render()
{
	ALLEGRO_BITMAP* previousTarget = DISPLAY->GetCurrentTarget();

	if( Size.X == 0 || Size.Y == 0 )
	{
		return;
	}

	if( controlArea == nullptr )
	{
		controlArea = al_create_bitmap( Size.X, Size.Y );
	} else if( al_get_bitmap_width( controlArea ) != Size.X || al_get_bitmap_height( controlArea ) != Size.Y ) {
		al_destroy_bitmap( controlArea );
		controlArea = al_create_bitmap( Size.X, Size.Y );
	}

	DISPLAY->SetTarget( controlArea );
	PreRender();
	OnRender();
	PostRender();

	DISPLAY->SetTarget( previousTarget );
	al_draw_bitmap( controlArea, Location.X, Location.Y, 0 );
}

void Control::PreRender()
{
	al_clear_to_color( BackgroundColour );
	//if( BackgroundColour.a != 0.0f )
	//{
	//	al_draw_filled_rectangle( 0, 0, Size.X, Size.Y, BackgroundColour );
	//}
}

void Control::OnRender()
{
	// Nothing specifically for the base control
}

void Control::PostRender()
{
	for( auto ctrlidx = Controls.begin(); ctrlidx != Controls.end(); ctrlidx++ )
	{
		Control* c = (Control*)*ctrlidx;
		c->Render();
	}
}

void Control::Update()
{
	for( auto ctrlidx = Controls.begin(); ctrlidx != Controls.end(); ctrlidx++ )
	{
		Control* c = (Control*)*ctrlidx;
		c->Update();
	}
}

void Control::UnloadResources()
{
}

Control* Control::operator[]( int Index )
{
	return Controls.at( Index );
}

Control* Control::FindControl( std::string ID )
{
	for( auto c = Controls.begin(); c != Controls.end(); c++ )
	{
		Control* ctrl = (Control*)*c;
		if( ctrl->Name == ID )
		{
			return ctrl;
		}
	}
	return nullptr;
}

Control* Control::GetParent()
{
	return owningControl;
}

Control* Control::GetRootControl()
{
	if( owningControl == nullptr )
	{
		return this;
	} else {
		return owningControl->GetRootControl();
	}
}

Form* Control::GetForm()
{
	Control* c = GetRootControl();
	if( dynamic_cast<Form*>( c ) != nullptr )
	{
		return (Form*)c;
	}
	return nullptr;
}
