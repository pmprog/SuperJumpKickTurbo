
#include "fighter_akuma.h"

void Akuma::Initialise( float Scale )
{
	currentPosition = new Vector2( 0, 0 );

	spriteSheet = new SpriteSheet( spLoadSurfaceZoom("Resource/akuma.png", (int)(SP_ONE * Scale)) );
	// Idle Frames
	spriteSheet->AddSprite( 12 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 52 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 92 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	spriteSheet->AddSprite( 133 * Scale, 6 * Scale, 38 * Scale, 52 * Scale );
	// Jump Frames
	spriteSheet->AddSprite( 208 * Scale, 0, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 256 * Scale, 378 * Scale, 32 * Scale, 64 * Scale );
	spriteSheet->AddSprite( 304 * Scale, 372 * Scale, 32 * Scale, 64 * Scale );
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
	//spriteSheet->AddSprite( 272 * Scale, 144 * Scale, 42 * Scale, 48 * Scale );
	//spriteSheet->AddSprite( 322 * Scale, 140 * Scale, 26 * Scale, 52 * Scale );
	//spriteSheet->AddSprite( 313 * Scale, 140 * Scale, 41 * Scale, 52 * Scale );
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
	animJumpLand = new Animation( spriteSheet, false, 40 );
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
	spriteSheet->DrawSprite( 15, ScreenX, ScreenY );
}

void Akuma::CharSelect_RenderName( int ScreenX, int ScreenY )
{
}

void Akuma::Fighter_Update()
{
	currentAnimation->Update();
	if( currentAnimation->HasEnded() )
	{
		if( currentAnimation == animJumpTakeOff )
		{
			currentAnimation = animJumpFloat;
			currentAnimation->Start();
		}
		if( currentAnimation == animJumpLand )
		{
			Fighter_SetState( Fighter::Idle );
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
	currentAnimation->DrawFrame( (int)currentPosition->X - ScreenOffsetX, (int)currentPosition->Y - ScreenOffsetY );
}
