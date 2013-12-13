
#pragma once

#include "../Framework/graphicslib.h"
#include <vector>

class SpriteSheet
{
	private:
		SDL_Surface* sheet;
		std::vector<SDL_Rect*> frames;

	public:
		SpriteSheet( SDL_Surface* SheetGraphic );
		SpriteSheet( SDL_Surface* SheetGraphic, int FrameWidth, int FrameHeight );
		SDL_Surface* GetSheet();

		int AddSprite( int FrameX, int FrameY, int FrameWidth, int FrameHeight );
		void DrawSprite( int FrameNumber, int ScreenX, int ScreenY );

};