
#pragma once

#include "spritesheet.h"

class Animation
{

	private:
		SpriteSheet* sprites;
		std::vector<int> frameList;
		bool animRepeats;
		bool animHasEnded;
		int animCurrentFrame;
		int animDelayTime;
		int animDelayTick;

	public:
		Animation( SpriteSheet* Sprites, bool LoopAnimation, int FrameDuration );

		void AddFrame( int SpriteIndex );

		void Start();
		void Update();
		void DrawFrame( int ScreenX, int ScreenY );
		bool Loops();
		bool HasEnded();
		int GetCurrentFrame();


};