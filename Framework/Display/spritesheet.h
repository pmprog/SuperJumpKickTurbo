
#pragma once

#include "../includes.h"
#include "../framework.h"
#include "bitmapcache.h"
#include "../Primitives/angle.h"

typedef struct SpriteSheetRegion
{
	int X;
	int Y;
	int Width;
	int Height;
} SpriteSheetRegion;

class SpriteSheet
{
	private:
		ALLEGRO_BITMAP* sheet;
		std::vector<SpriteSheetRegion*> frames;

	public:
		SpriteSheet( std::string Filename );
		SpriteSheet( std::string Filename, int FrameWidth, int FrameHeight );
		~SpriteSheet();

		ALLEGRO_BITMAP* GetSheet();

		int AddSprite( int FrameX, int FrameY, int FrameWidth, int FrameHeight );
		void DrawSprite( int FrameNumber, int ScreenX, int ScreenY );
		void DrawSprite( int FrameNumber, int ScreenX, int ScreenY, float ScaleX, float ScaleY, Angle* Rotation );

		void DrawSpritePortion( int FrameNumber, int ScreenX, int ScreenY, int FrameX, int FrameY, int Width, int Height );

		SpriteSheetRegion* GetFrame( int FrameNumber );

};
