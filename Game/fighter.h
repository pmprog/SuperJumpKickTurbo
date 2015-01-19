
#pragma once

#include <string>
#include "../Framework/Primitives/vector2.h"
#include "../Framework/Primitives/box.h"
#include "../Framework/Display/animation.h"

#ifndef Arena
class Arena;
#endif

#define FIGHTER_MAXIMUM_ROLLBACK_STATES		200

class Fighter
{
	public:
		enum FighterController
		{
			NoControls,
			LocalKeyboardP1,
			LocalKeyboardP2,
			LocalJoystickP1,
			LocalJoystickP2,
			NetworkClient,
			CPU_Easy,
			CPU_Medium,
			CPU_Hard
		};

		enum FighterStates
		{
			Idle,
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

		typedef struct FighterSaveState
		{
			uint64_t FrameCount;
			FighterStates State;
			int StateTime;
			Animation* Anim;
			float X;
			float Y;
			bool FaceLeft;
		} FighterSaveState;

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

		FighterSaveState RollbackStates[FIGHTER_MAXIMUM_ROLLBACK_STATES];

		Box* CollisionBoxToScreenBox(Box* Source);


	public:
		Arena* currentArena;
		bool FighterHit;

		std::string CharacterName;
		std::string PlayerName;

		FighterController Controller;
		static Network* NetworkController;

		Fighter( FighterController Controls, std::string Config, Arena* FightArena, bool AlternativeSprites );

		void CharSelect_RenderProfileIcon( int ScreenX, int ScreenY );

		void Fighter_Update( bool IgnoreCollisions );
		FighterStates Fighter_GetState();
		void Fighter_SetState( FighterStates NewState );
		int Fighter_GetStateTime();
		void Fighter_SetStateTime( int NewStateTime );

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

		void AI_Update( int Skill );

		void State_Clear();
		void State_Save(uint64_t FrameCount);
		bool State_Load(uint64_t FrameCount);

};
