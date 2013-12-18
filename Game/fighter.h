
#pragma once

#include "../Library/vector2.h"

class Fighter
{

	public:
		enum FighterStates
		{
			Idle = 0,
			Jump,
			Kick,
			BackJump,
			Super,
			Ultra,
			Victor,
			Loser
		};

		virtual void Initialise( float Scale ) = 0;

		virtual void CharSelect_RenderProfileIcon( int ScreenX, int ScreenY ) = 0;
		virtual void CharSelect_RenderName( int ScreenX, int ScreenY ) = 0;

		virtual void Fighter_Update() = 0;
		virtual void Fighter_SetState( int NewState ) = 0;

		virtual Vector2* Fighter_GetPosition() = 0;
		virtual void Fighter_SetPosition( float X, float Y ) = 0;
		virtual void Fighter_SetPosition( Vector2* NewPosition ) = 0;
		virtual void Fighter_Render( int ScreenOffsetX, int ScreenOffsetY ) = 0;

		virtual bool Fighter_IsFacingLeft() = 0;
		virtual void Fighter_SetFacing( bool FacingLeft ) = 0;

		virtual void Fighter_JumpPressed() = 0;
		virtual void Fighter_KickPressed() = 0;
		virtual void Fighter_SuperPressed() = 0;

};
