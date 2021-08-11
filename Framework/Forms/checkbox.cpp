
#include "checkbox.h"

CheckBox::CheckBox( Control* Owner ) : Control( Owner ), imagechecked(nullptr), imageunchecked(nullptr), Checked(false)
{
	CanFocus = true;
	LoadResources();
}

CheckBox::~CheckBox()
{
}

void CheckBox::LoadResources()
{
}

void CheckBox::EventOccured( Event* e )
{
	Control::EventOccured( e );

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseDown )
	{
		// buttonclick->PlaySound();
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseClick )
	{
		Checked = !Checked;
		Event* ce = new Event();
		ce->Type = e->Type;
		memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
		ce->Data.Forms.EventFlag = FormEventType::CheckBoxChange;
		FRAMEWORK->PushEvent( ce );
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::KeyDown )
	{
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_SPACE || e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_ENTER )
		{
			Checked = !Checked;
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::CheckBoxChange;
			FRAMEWORK->PushEvent( ce );
		}
	}
}

void CheckBox::OnRender()
{
	ALLEGRO_BITMAP* useimage;

	LoadResources();

	useimage = ( Checked ? imagechecked : imageunchecked);

	if( useimage != nullptr )
	{
		int bmpw = al_get_bitmap_width( useimage );
		int bmph = al_get_bitmap_height( useimage );
		if( bmpw == Size.X && bmph == Size.Y )
		{
			al_draw_bitmap( useimage, 0, 0, 0 );
		} else {
			al_draw_scaled_bitmap( useimage, 0, 0, bmpw, bmph, 0, 0, this->Size.X, this->Size.Y, 0 );
		}
	} else {
		if( Size.X > 8 && Size.Y > 8 )
		{
			if( selectedDraw )
			{
				al_draw_filled_rectangle( 1, 1, Size.X - 2, Size.Y - 2, ForegroundColour );
				al_draw_filled_rectangle( 2, 2, Size.X - 3, Size.Y - 3, al_map_rgb( 255, 255, 255 ) );
			} else {
				al_draw_filled_rectangle( 1, 1, Size.X - 2, Size.Y - 2, ForegroundColour );
				al_draw_filled_rectangle( 2, 2, Size.X - 3, Size.Y - 3, al_map_rgb( 0, 0, 0 ) );
			}
			if( Checked )
			{
				al_draw_filled_rectangle( 6, 6, Size.X - 7, Size.Y - 7, ForegroundColour );
			}
		} else {
			if( Checked )
			{
				al_clear_to_color( ForegroundColour );
			}
		}
	}
}

void CheckBox::Update()
{
	if( IsFocused() )
	{
		selectedTimer = (selectedTimer + 1) % CHECKBOX_SELECT_TOGGLE_TIME;
		if( selectedTimer == 0 )
		{
			selectedDraw = !selectedDraw;
		}
	} else {
		selectedDraw = false;
	}
}

void CheckBox::UnloadResources()
{
	if( imagechecked != nullptr )
	{
		al_destroy_bitmap( imagechecked );
		imagechecked = nullptr;
	}
	if( imageunchecked != nullptr )
	{
		al_destroy_bitmap( imageunchecked );
		imageunchecked = nullptr;
	}
	Control::UnloadResources();
}
