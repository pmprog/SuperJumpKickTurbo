
#pragma once

#include "fighter.h"
#include "../Library/animation.h"

class Akuma : public Fighter
{

	private:
		SpriteSheet* spriteSheet;
		Animation* animIdle;
		Animation* animJumpTakeOff;
		Animation* animJumpFloat;
		Animation* animJumpLand;
		Animation* animKick;
		Animation* animSuper;
		Animation* animUltra;
		Animation* animKnockDown;
		Animation* animKOLand;
		Animation* animKO;
		Animation* animWin;


		Fighter::FighterStates currentState;
		Animation* currentAnimation;
		Vector2* currentPosition;

	public:
		virtual void Initialise( float Scale );

		virtual void CharSelect_RenderProfileIcon( int ScreenX, int ScreenY );
		virtual void CharSelect_RenderName( int ScreenX, int ScreenY );

		virtual void Fighter_Update();
		virtual void Fighter_SetState( int NewState );

		virtual Vector2* Fighter_GetPosition();
		virtual void Fighter_SetPosition( float X, float Y );
		virtual void Fighter_SetPosition( Vector2* NewPosition );
		virtual void Fighter_Render( int ScreenOffsetX, int ScreenOffsetY );

};
