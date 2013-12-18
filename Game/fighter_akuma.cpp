
#include "fighter_akuma.h"
#include "../Framework/framework.h"

void Akuma::Initialise( float Scale )
{
	currentPosition = new Vector2( 0, 0 );
	currentFaceLeft = false;
	currentScale = Scale;

	spriteSheet = new SpriteSheet( spLoadSurfaceZoom("Resource/akuma.png", (int)(SP_ONE * Scale)) );
	// Idle Frames
	spriteSheet->AddSprite( 12 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 52 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 92 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 133 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	// Jump Frames
	spriteSheet->AddSprite( 208 * Scale, 0, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 256 * Scale, 0 * Scale, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 304 * Scale, 0 * Scale, 32 * Scale, 64 * Scale );
	// Kick Frames
	spriteSheet->AddSprite( 384 * Scale, 64 * Scale, 48 * Scale, 64 * Scale );
	// Super Frames
	spriteSheet->AddSprite( 94 * Scale, 64 * Scale, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 262 * Scale, 64 * Scale, 48 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 132 * Scale, 64 * Scale, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 170 * Scale, 64 * Scale, 46 * Scale, 64 * Scale );
	// KnockDown/KO Frames
	spriteSheet->AddSprite( 18 * Scale, 192 * Scale, 54 * Scale, 40 * Scale );
	spriteSheet->AddSprite( 74 * Scale, 192 * Scale, 50 * Scale, 38 * Scale );
	spriteSheet->AddSprite( 128 * Scale, 192 * Scale, 54 * Scale, 38 * Scale );
	// Win Frames
	spriteSheet->AddSprite( 360 * Scale, 144 * Scale, 44 * Scale, 48 * Scale );
	spriteSheet->AddSprite( 410 * Scale, 144 * Scale, 44 * Scale, 48 * Scale );
	// Profile Icon
	spriteSheet->AddSprite( 352 * Scale, 16 * Scale, 16 * Scale, 16 * Scale );
	// Profile Name
	// Ultra Frames

	animIdle = new Animation( spriteSheet, true, 9 );
	for( int i = 0; i <= 3; i++ )
	{
		animIdle->AddFrame( i );
	}

	animJumpTakeOff = new Animation( spriteSheet, false, 40 );
	animJumpTakeOff->AddFrame( 4 );
	animJumpFloat = new Animation( spriteSheet, true, 40 );
	animJumpFloat->AddFrame( 5 );
	animJumpLand = new Animation( spriteSheet, true, 40 );
	animJumpLand->AddFrame( 6 );

	animKick = new Animation( spriteSheet, true, 9 );
	animKick->AddFrame( 7 );

	animSuper = new Animation( spriteSheet, true, 9 );
	for( int i = 8; i <= 11; i++ )
	{
		animSuper->AddFrame( i );
	}

	animKnockDown = new Animation( spriteSheet, true, 9 );
	animKnockDown->AddFrame( 12 );
	animKOLand = new Animation( spriteSheet, false, 5 );
	animKOLand->AddFrame( 13 );
	animKO = new Animation( spriteSheet, true, 9 );
	animKO->AddFrame( 14 );

	animWin = new Animation( spriteSheet, true, 8 );
	for( int i = 0; i <= 3; i++ )
	{
		animWin->AddFrame( i );
	}
	for( int i = 1; i < 6; i++ )
	{
		animWin->AddFrame( 15 );
		animWin->AddFrame( 16 );
	}

	Fighter_SetState( Fighter::Idle );
}


void Akuma::CharSelect_RenderProfileIcon( int ScreenX, int ScreenY )
{
	spSetVerticalOrigin( SP_CENTER );
	spSetHorizontalOrigin( SP_CENTER );
	spriteSheet->DrawSprite( 15, ScreenX, ScreenY, 1.0, 1.0, 0 );
}

void Akuma::CharSelect_RenderName( int ScreenX, int ScreenY )
{
}

void Akuma::Fighter_Update()
{
	currentAnimation->Update();
	currentStateTime++;

	switch( currentState )
	{
		case Fighter::Idle:
			break;
		case Fighter::Jump:
			if( currentStateTime < AKUMA_JUMP_FRAMES )
			{
				currentPosition->Y += (currentScale * AKUMA_JUMP_SPEED) * ((AKUMA_JUMP_FRAMES - currentStateTime) / AKUMA_JUMP_FRAMES);
			} else {
				currentPosition->Y -= (currentScale * AKUMA_JUMP_SPEED) * ((currentStateTime - AKUMA_JUMP_FRAMES) / AKUMA_JUMP_FRAMES);
				if( currentStateTime == (int)(AKUMA_JUMP_FRAMES * 0.9f) )
				{
					currentAnimation = animJumpLand;
					currentAnimation->Start();
				}
				if( currentPosition->Y <= 0 )
				{
					currentPosition->Y = 0;
					Fighter_SetState( Fighter::Idle );
				}
			}
			break;
		case Fighter::BackJump:
			if( currentStateTime < AKUMA_BACKJUMP_FRAMES )
			{
				currentPosition->Y += (currentScale * AKUMA_BACKJUMP_SPEED) * ((AKUMA_BACKJUMP_FRAMES - currentStateTime) / AKUMA_BACKJUMP_FRAMES);
				currentPosition->X -= (currentFaceLeft ? -1 : 1) * (currentScale * AKUMA_BACKJUMP_RSPEED);
			} else {
				currentPosition->Y -= (currentScale * AKUMA_BACKJUMP_SPEED) * ((currentStateTime - AKUMA_BACKJUMP_FRAMES) / AKUMA_BACKJUMP_FRAMES);
				currentPosition->X -= (currentFaceLeft ? -1 : 1) * (currentScale * AKUMA_BACKJUMP_RSPEED);
				if( currentStateTime == (int)(AKUMA_BACKJUMP_FRAMES * 0.9f) )
				{
					currentAnimation = animJumpLand;
					currentAnimation->Start();
				}
				if( currentPosition->Y <= 0 )
				{
					currentPosition->Y = 0;
					Fighter_SetState( Fighter::Idle );
				}
			}
			break;
		case Fighter::Kick:
			currentPosition->Y -= (currentScale * AKUMA_KICK_SPEED);
			currentPosition->X -= (currentFaceLeft ? 1 : -1) * (currentScale * AKUMA_KICK_RSPEED);
			if( currentPosition->Y <= 0 )
			{
				currentPosition->Y = 0;
				Fighter_SetState( Fighter::Idle );
			}
			break;
		case Fighter::Super:
			break;
		case Fighter::Loser:
			break;
		case Fighter::Victor:
			break;
//		case Fighter::Ultra:
//			break;
	}


	if( currentAnimation->HasEnded() )
	{
		if( currentAnimation == animJumpTakeOff )
		{
			currentAnimation = animJumpFloat;
			currentAnimation->Start();
		}
		if( currentAnimation == animKOLand )
		{
			currentAnimation = animKO;
			currentAnimation->Start();
		}
	}
}

void Akuma::Fighter_SetState( int NewState )
{
	// Exit State Code
/*
	switch( currentState )
	{
	}
	*/

	currentState = (Fighter::FighterStates)NewState;
	currentStateTime = 0;

	// Enter State Code
	switch( NewState )
	{
		case Fighter::Idle:
			currentAnimation = animIdle;
			currentAnimation->Start();
			break;
		case Fighter::Jump:
		case Fighter::BackJump:
			currentAnimation = animJumpTakeOff;
			currentAnimation->Start();
			break;
		case Fighter::Kick:
			currentAnimation = animKick;
			currentAnimation->Start();
			break;
		case Fighter::Super:
			currentAnimation = animSuper;
			currentAnimation->Start();
			break;
		case Fighter::Victor:
			currentAnimation = animWin;
			currentAnimation->Start();
			break;
		case Fighter::Loser:
			currentAnimation = animKnockDown;
			currentAnimation->Start();
			break;
	}
}


Vector2* Akuma::Fighter_GetPosition()
{
	return currentPosition;
}

void Akuma::Fighter_SetPosition( float X, float Y )
{
	currentPosition->X = X;
	currentPosition->Y = Y;
}

void Akuma::Fighter_SetPosition( Vector2* NewPosition )
{
	currentPosition->X = NewPosition->X;
	currentPosition->Y = NewPosition->Y;
}

void Akuma::Fighter_Render( int ScreenOffsetX, int ScreenOffsetY )
{
	spSetVerticalOrigin( SP_BOTTOM );
	spSetHorizontalOrigin( SP_CENTER );

	int screenY = Framework::System->GetDisplayHeight() * 0.9;	// Y @ 0 is 10% screen height from the bottom of the screen
	screenY = screenY - currentPosition->Y + ScreenOffsetY;

	currentAnimation->DrawFrame( (int)currentPosition->X - ScreenOffsetX, screenY, currentFaceLeft, false );
}

bool Akuma::Fighter_IsFacingLeft()
{
	return currentFaceLeft;
}

void Akuma::Fighter_SetFacing( bool FacingLeft )
{
	if( currentState == Fighter::Idle )
	{
		currentFaceLeft = FacingLeft;
	}
}

void Akuma::Fighter_JumpPressed()
{
	switch( currentState )
	{
		case Fighter::Idle:
			Fighter_SetState( Fighter::Jump );
			break;
	}
}

void Akuma::Fighter_KickPressed()
{
	switch( currentState )
	{
		case Fighter::Idle:
			Fighter_SetState( Fighter::BackJump );
			break;
		case Fighter::Jump:
		case Fighter::BackJump:
			Fighter_SetState( Fighter::Kick );
			break;

	}
}

void Akuma::Fighter_SuperPressed()
{
	switch( currentState )
	{
		case Fighter::Idle:
		case Fighter::Jump:
		case Fighter::BackJump:
		case Fighter::Kick:
			Fighter_SetState( Fighter::Super );
			break;

	}
}

