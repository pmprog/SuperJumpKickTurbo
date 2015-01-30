
#include "hscrollbar.h"

HScrollBar::HScrollBar( Control* Owner ) : Control( Owner ), Maximum(10), Minimum(0), Value(0), capture(false), LargeChange(2)
{
	//LoadResources();
}

HScrollBar::~HScrollBar()
{
}

void HScrollBar::LoadResources()
{
}

void HScrollBar::EventOccured( Event* e )
{
	Control::EventOccured( e );

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseDown )
	{
		int segments = (Maximum - Minimum) + 1;
		float segmentsize = Size.X / (float)segments;
		float grippersize = segmentsize;
		if (grippersize < 16.0f)
		{
			grippersize = 16.0f;
			segmentsize = (Size.X - grippersize) / (float)(segments - 1);
		}

		if( e->Data.Forms.MouseInfo.X >= (segmentsize * (Value - Minimum)) + grippersize )
		{
			Value = Maths::Min(Maximum, Value + LargeChange);
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		} else if ( e->Data.Forms.MouseInfo.X <= segmentsize * (Value - Minimum) ) {
			Value = Maths::Max(Minimum, Value - LargeChange);
			Event* ce = new Event();
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
		float segmentsize = Size.X / (float)segments;
		Value = Maths::Max(Minimum, Minimum + (int)(e->Data.Forms.MouseInfo.X / segmentsize));
		Event* ce = new Event();
		ce->Type = e->Type;
		memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
		ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
		FRAMEWORK->PushEvent( ce );
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::KeyPress )
	{
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_LEFT && Value > Minimum )
		{
			Value--;
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ScrollBarChange;
			FRAMEWORK->PushEvent( ce );
		}
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_RIGHT && Value < Maximum )
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

void HScrollBar::OnRender()
{
	//LoadResources();

	int segments = (Maximum - Minimum) + 1;
	float segmentsize = Size.X / (float)segments;
	float grippersize = segmentsize;
	if (grippersize < 16.0f)
	{
		grippersize = 16.0f;
		segmentsize = (Size.X - grippersize) / (float)(segments - 1);
	}

	int xpos = segmentsize * (Value - Minimum);
	al_draw_filled_rectangle( xpos, 0, xpos + grippersize, Size.Y, ForegroundColour );
	if( selectedDraw )
	{
		al_draw_filled_rectangle( xpos, 0, xpos + grippersize, Size.Y, al_map_rgba( 128, 128, 128, 128 ) );
	}

}

void HScrollBar::Update()
{
	if( IsFocused() )
	{
		selectedTimer = (selectedTimer + 1) % HSCROLLBAR_SELECT_TOGGLE_TIME;
		if( selectedTimer == 0 )
		{
			selectedDraw = !selectedDraw;
		}
	} else {
		selectedDraw = false;
	}
}

void HScrollBar::UnloadResources()
{
	Control::UnloadResources();
}