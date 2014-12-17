
#pragma once

#include <string>
#include "../Framework/Primitives/vector2.h"
#include "../Framework/Primitives/box.h"
#include "../Framework/Display/animation.h"

#ifndef Arena
class Arena;
#endif

class Fighter
{
	public:
		enum FighterStates
		{
			Idle ,
			Jump,
			Kick,
			BackJump,
			Super,
			Ultra,
			Knockdown,
			Floored,
			Victor,
			Loser
		};

	private:
		SpriteSheet* spriteSheet;
		Animation* animIdle;
		std::vector<Box*> collisionIdle;
		Animation* animJumpTakeOff;
		std::vector<Box*> collisionJumpTakeOff;
		Animation* animJumpFloat;
		std::vector<Box*> collisionJumpFloat;
		Animation* animJumpLand;
		std::vector<Box*> collisionJumpLand;
		Animation* animKick;
		std::vector<Box*> collisionKick;
		std::vector<Box*> attackKick;
		Animation* animSuper;
		Animation* animKnockDown;
		Animation* animKnockDownLand;
		Animation* animKnockedOut;
		Animation* animWin;

		float currentScale;
		FighterStates currentState;
		int currentStateTime;
		Animation* currentAnimation;
		Vector2* currentPosition;
		bool currentFaceLeft;

		float jumpFrames;
		float jumpSpeed;
		float jumpBackFrames;
		float jumpBackVSpeed;
		float jumpBackHSpeed;
		float kickVSpeed;
		float kickHSpeed;
		int arenaWidth;

		Box* CollisionBoxToScreenBox(Box* Source);


	public:
		bool FighterHit;
		std::string CharacterName;

		Fighter( std::string Config, int ArenaWidth, bool AlternativeSprites );

		void CharSelect_RenderProfileIcon( int ScreenX, int ScreenY );

		void Fighter_Update( Arena* Current );
		int Fighter_GetState();
		void Fighter_SetState( int NewState );

		Vector2* Fighter_GetPosition();
		void Fighter_SetPosition( float X, float Y );
		void Fighter_SetPosition( Vector2* NewPosition );
		void Fighter_Render( int ScreenOffsetX, int ScreenOffsetY );

		bool Fighter_IsFacingLeft();
		void Fighter_SetFacing( bool FacingLeft );

		void Fighter_JumpPressed();
		void Fighter_KickPressed();
		void Fighter_SuperPressed();

		Box* Fighter_GetCurrentHitBox();

};
