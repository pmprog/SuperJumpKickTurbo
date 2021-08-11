
#include "vscrollbar.h"

VScrollBar::VScrollBar( Control* Owner ) : Control( Owner ), capture(false), Minimum(0), Maximum(10), Value(0), LargeChange(2)
{
	//LoadResources();
}

VScrollBar::~VScrollBar()
{
}

void VScrollBar::LoadResources()
{
}

void VScrollBar::EventOccured( Event* e )
{
	Event* ce;

	Control::EventOccured( e );

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseDown )
	{
		int segments = (Maximum - Minimum) + 1;
		float segmentsize = Size.Y / (float)segments;
		float grippersize = segmentsize;
		if (grippersize < 16.0f)
		{
			grippersize = 16.0f;
			segmentsize = (Size.Y - grippersize) / (float)(segments - 1);
		}

		if( e->Data.Forms.MouseInfo.Y >= (segmentsize * (Value - Minimum)) + grippersize )
		{
			Value = Maths::Min(Maximum, Value + LargeChange);
			ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		} else if ( e->Data.Forms.MouseInfo.Y <= segmentsize * (Value - Minimum) ) {
			Value = Maths::Max(Minimum, Value - LargeChange);
			ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		} else {
			capture = true;
		}
	}

	if( e->Type == EVENT_FORM_INTERACTION && (capture || e->Data.Forms.RaisedBy == this) && e->Data.Forms.EventFlag == FormEventType::MouseUp )
	{
		capture = false;
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseMove && capture )
	{
		int segments = (Maximum - Minimum) + 1;
		float segmentsize = Size.Y / (float)segments;
		Value = Maths::Max(Minimum, Minimum + (int)(e->Data.Forms.MouseInfo.Y / segmentsize));
		ce = new Event();
		ce->Type = e->Type;
		memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
		ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
		FRAMEWORK->PushEvent( ce );
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::KeyPress )
	{
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_UP && Value > Minimum )
		{
			Value--;
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		}
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_DOWN && Value < Maximum )
		{
			Value++;
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		}
	}
}

void VScrollBar::OnRender()
{
	//LoadResources();

	int segments = (Maximum - Minimum) + 1;
	float segmentsize = Size.Y / (float)segments;
	float grippersize = segmentsize;
	if (grippersize < 16.0f)
	{
		grippersize = 16.0f;
		segmentsize = (Size.Y - grippersize) / (float)(segments - 1);
	}

	int ypos = segmentsize * (Value - Minimum);
	al_draw_filled_rectangle( 0, ypos, Size.X, ypos + grippersize, ForegroundColour );
	if( selectedDraw )
	{
		al_draw_filled_rectangle( 0, ypos, Size.X, ypos + grippersize, al_map_rgba( 128, 128, 128, 128 ) );
	}
}

void VScrollBar::Update()
{
	if( IsFocused() )
	{
		selectedTimer = (selectedTimer + 1) % VSCROLLBAR_SELECT_TOGGLE_TIME;
		if( selectedTimer == 0 )
		{
			selectedDraw = !selectedDraw;
		}
	} else {
		selectedDraw = false;
	}
}

void VScrollBar::UnloadResources()
{
	Control::UnloadResources();
}
