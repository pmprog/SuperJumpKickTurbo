
#include "animation.h"

Animation::Animation( SpriteSheet* Sprites, bool LoopAnimation, int FrameDuration )
{
	sprites = Sprites;
	animRepeats = LoopAnimation;
	animDelayTime = FrameDuration;
	Start();
}

void Animation::AddFrame( int SpriteIndex )
{
	frameList.push_back( SpriteIndex );
}

void Animation::Start()
{
	animDelayTick = 0;
	animCurrentFrame = 0;
	animHasEnded = false;
}

void Animation::Update()
{
	animDelayTick = (animDelayTick + 1) % animDelayTime;
	if( animDelayTick == 0 )
	{
		animCurrentFrame = (animCurrentFrame + 1) % frameList.size();
		if( animCurrentFrame == 0 && !animRepeats )
		{
			animHasEnded = true;
		}
	}
}

void Animation::DrawFrame( int ScreenX, int ScreenY )
{
	sprites->DrawSprite( frameList.at(animCurrentFrame), ScreenX, ScreenY );
}

bool Animation::Loops()
{
	return animRepeats;
}

bool Animation::HasEnded()
{
	return animHasEnded;
}

int Animation::GetCurrentFrame()
{
	return animCurrentFrame;
}
