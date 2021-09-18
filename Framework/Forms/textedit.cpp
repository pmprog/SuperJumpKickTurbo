
#include "textedit.h"
#include <iostream>
#include <algorithm>

TextEdit::TextEdit( Control* Owner, std::string Text, TTFFont* Font ) : Control( Owner ), caretDraw(false), caretTimer(0), text( Text ), font( Font ), editting(false), editShift(false), editAltGr(false), SelectionStart(Text.length()), TextHAlign( HorizontalAlignment::Left ), TextVAlign( VerticalAlignment::Centre )
{
	CanFocus = true;
}

TextEdit::~TextEdit()
{
}

void TextEdit::EventOccured( Event* e )
{
	std::string keyname;

	Control::EventOccured( e );

	if( e->Type == EVENT_FORM_INTERACTION )
	{
		if( e->Data.Forms.RaisedBy == this )
		{
			if( e->Data.Forms.EventFlag == FormEventType::GotFocus )
			{
				editting = true;
				e->Handled = true;
			}
			// if( !IsFocused() && ( e->Data.Forms.EventFlag == FormEventType::MouseClick || e->Data.Forms.EventFlag == FormEventType::KeyDown ) )
			if( e->Data.Forms.EventFlag == FormEventType::MouseClick )
			{
				Focus();
				e->Handled = true;
			}
			if( e->Data.Forms.EventFlag == FormEventType::LostFocus )
			{
				editting = false;
				RaiseEvent( FormEventType::TextEditFinish );
				e->Handled = true;
			}
		} else if( e->Data.Forms.EventFlag == FormEventType::MouseClick ) {
			editting = false;
			RaiseEvent( FormEventType::TextEditFinish );
		}

		if( e->Data.Forms.EventFlag == FormEventType::KeyPress && !editting && IsFocused() )
		{
			editting = true;
			e->Handled = true;
			return;
		}

		if( e->Data.Forms.EventFlag == FormEventType::KeyPress && editting )
		{
			switch( e->Data.Forms.KeyInfo.KeyCode )
			{
				case ALLEGRO_KEY_BACKSPACE:
					if( SelectionStart > 0 )
					{
						text.erase( text.begin() + SelectionStart - 1, text.begin() + SelectionStart );
						SelectionStart--;
						RaiseEvent( FormEventType::TextChanged );
					}
					e->Handled = true;
					break;
				case ALLEGRO_KEY_DELETE:
					if( SelectionStart < (int)text.length() )
					{
						text.erase( text.begin() + SelectionStart, text.begin() + SelectionStart + 1 );
						RaiseEvent( FormEventType::TextChanged );
					}
					e->Handled = true;
					break;
				case ALLEGRO_KEY_LEFT:
					if( SelectionStart > 0 )
					{
						SelectionStart--;
					}
					e->Handled = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					if( SelectionStart < (int)text.length() )
					{
						SelectionStart++;
					}
					e->Handled = true;
					break;
				case ALLEGRO_KEY_LSHIFT:
				case ALLEGRO_KEY_RSHIFT:
					editShift = true;
					break;
				case ALLEGRO_KEY_ALTGR:
					editAltGr = true;
					break;

				case ALLEGRO_KEY_HOME:
					SelectionStart = 0;
					e->Handled = true;
					break;
				case ALLEGRO_KEY_END:
					SelectionStart = text.length();
					e->Handled = true;
					break;

				case ALLEGRO_KEY_ENTER:
				case ALLEGRO_KEY_ESCAPE:
					editting = false;
					RaiseEvent( FormEventType::TextEditFinish );
					break;

				default:
					ALLEGRO_USTR* convert = al_ustr_new("");
					al_ustr_append_chr( convert, e->Data.Forms.KeyInfo.UniChar );
					if( convert->slen == 1 && al_cstr(convert)[0] != 0 )
					{
						text.insert( SelectionStart, al_cstr(convert) );
						SelectionStart++;
						RaiseEvent( FormEventType::TextChanged );
					}
			}
		}

		if( e->Data.Forms.EventFlag == FormEventType::KeyUp && editting )
		{

			switch( e->Data.Forms.KeyInfo.KeyCode )
			{
				case ALLEGRO_KEY_LSHIFT:
				case ALLEGRO_KEY_RSHIFT:
					editShift = false;
					e->Handled = true;
					break;
				case ALLEGRO_KEY_ALTGR:
					editAltGr = false;
					e->Handled = true;
					break;
			}
		}
	}

}

void TextEdit::OnRender()
{
	int xpos;
	int ypos;

	al_draw_line( 0, Size.Y - 1, Size.X, Size.Y - 1, ForegroundColour, 1 );

	switch( TextHAlign )
	{
		case HorizontalAlignment::Left:
			xpos = 0;
			break;
		case HorizontalAlignment::Centre:
			xpos = (Size.X / 2) - (font->GetFontWidth( text ) / 2);
			break;
		case HorizontalAlignment::Right:
			xpos = Size.X - font->GetFontWidth( text );
			break;
	}

	switch( TextVAlign )
	{
		case VerticalAlignment::Top:
			ypos = 0;
			break;
		case VerticalAlignment::Centre:
			ypos = (Size.Y / 2) - (font->GetFontHeight() / 2);
			break;
		case VerticalAlignment::Bottom:
			ypos = Size.Y - font->GetFontHeight();
			break;
	}

	if( editting )
	{
		int cxpos = xpos + font->GetFontWidth( text.substr( 0, SelectionStart ) ) + 1;

		if( cxpos < 0 )
		{
			xpos += cxpos;
			cxpos = xpos + font->GetFontWidth( text.substr( 0, SelectionStart ) ) + 1;
		}
		if( cxpos > Size.X )
		{
			xpos -= cxpos - Size.X;
			cxpos = xpos + font->GetFontWidth( text.substr( 0, SelectionStart ) ) + 1;
		}

		if( caretDraw )
		{
			al_draw_line( cxpos, ypos, cxpos, ypos + font->GetFontHeight(), ForegroundColour, 1 );
		}
	}

	font->DrawString( xpos, ypos, text, FontHAlign::LEFT, ForegroundColour );
}

void TextEdit::Update()
{
	if( editting )
	{
		caretTimer = (caretTimer + 1) % TEXTEDITOR_CARET_TOGGLE_TIME;
		if( caretTimer == 0 )
		{
			caretDraw = !caretDraw;
		}
	}
}

void TextEdit::UnloadResources()
{
}

std::string TextEdit::GetText()
{
	return text;
}

void TextEdit::SetText( std::string Text )
{
	text = Text;
	SelectionStart = text.length();
	RaiseEvent( FormEventType::TextChanged );
}

void TextEdit::RaiseEvent( FormEventType )
{
	Event* ce = new Event();
	ce->Type = EVENT_FORM_INTERACTION;
	memset( (void*)&(ce->Data.Forms), 0, sizeof( FRAMEWORK_FORMS_EVENT ) );
	ce->Data.Forms.RaisedBy = this;
	ce->Data.Forms.EventFlag = FormEventType::TextChanged;
	FRAMEWORK->PushEvent( ce );
}

void TextEdit::BeginEdit()
{
	editting = true;
}
