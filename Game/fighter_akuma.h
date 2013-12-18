
#pragma once

#include "fighter.h"
#include "../Library/animation.h"

class Akuma : public Fighter
{

	#define AKUMA_JUMP_FRAMES				100.0f
	#define AKUMA_JUMP_SPEED				3.0f
	#define AKUMA_BACKJUMP_FRAMES		50.0f
	#define AKUMA_BACKJUMP_SPEED		2.0f
	#define AKUMA_BACKJUMP_RSPEED		1.0f
	#define AKUMA_KICK_SPEED				2.0f
	#define AKUMA_KICK_RSPEED				1.5f

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

		float currentScale;
		Fighter::FighterStates currentState;
		int currentStateTime;
		Animation* currentAnimation;
		Vector2* currentPosition;
		bool currentFaceLeft;

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

		virtual bool Fighter_IsFacingLeft();
		virtual void Fighter_SetFacing( bool FacingLeft );

		virtual void Fighter_JumpPressed();
		virtual void Fighter_KickPressed();
		virtual void Fighter_SuperPressed();

};
