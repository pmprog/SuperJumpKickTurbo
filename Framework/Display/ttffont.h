
#pragma once

#include "../includes.h"

struct FontHAlign
{
	enum FontHorizontalAlignment
	{
		LEFT,
		CENTRE,
		RIGHT
	};
};

class TTFFont
{

	private:
		ALLEGRO_FONT* fontobject;
		int fontheight;

	public:
		TTFFont( std::string Filename, int Size );
		~TTFFont();

		virtual void DrawString( int X, int Y, std::string Text, FontHAlign::FontHorizontalAlignment Alignment, ALLEGRO_COLOR Colour );

		virtual int GetFontHeight();
		virtual int GetFontWidth(std::string Text);
};
