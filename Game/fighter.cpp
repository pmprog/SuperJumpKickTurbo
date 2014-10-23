
#include "fighter.h"

void Fighter::Initialise(std::string Config)
{
	ConfigFile* cfg = new ConfigFile( Config );

	CharacterName.clear();
	CharacterName.append( cfg->GetQuickStringValue("Name", "Unknown")->c_str() );

	spriteSheet = new SpriteSheet( *cfg->GetQuickStringValue("Sprites", "") );

	// Add profile sprite
	spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Profile", 0, 0), cfg->GetQuickIntegerValue( "Profile", 1, 0), cfg->GetQuickIntegerValue( "Profile", 2, 0), cfg->GetQuickIntegerValue( "Profile", 3, 0) )

}

void Fighter::CharSelect_RenderProfileIcon(int ScreenX, int ScreenY)
{
	// TODO: Write render code
	spriteSheet->DrawSprite( 0, ScreenX, ScreenY );
}

void Fighter::CharSelect_RenderName(int ScreenX, int ScreenY)
{
	// TODO: Write render code
}

void Fighter::Fighter_Update()
{
	currentAnimation->Update();
	currentStateTime++;

	switch (currentState)
	{
	case Fighter::Idle:
		break;
	case Fighter::Jump:
		//if (currentStateTime < AKUMA_JUMP_FRAMES)
		//{
		//	currentPosition->Y += (currentScale * AKUMA_JUMP_SPEED) * ((AKUMA_JUMP_FRAMES - currentStateTime) / AKUMA_JUMP_FRAMES);
		//}
		//else {
		//	currentPosition->Y -= (currentScale * AKUMA_JUMP_SPEED) * ((currentStateTime - AKUMA_JUMP_FRAMES) / AKUMA_JUMP_FRAMES);
		//	if (currentStateTime == (int)(AKUMA_JUMP_FRAMES * 0.9f))
		//	{
		//		currentAnimation = animJumpLand;
		//		currentAnimation->Start();
		//	}
		//	if (currentPosition->Y <= 0)
		//	{
		//		currentPosition->Y = 0;
		//		Fighter_SetState(Fighter::Idle);
		//	}
		//}
		break;
	case Fighter::BackJump:
		//if (currentStateTime < AKUMA_BACKJUMP_FRAMES)
		//{
		//	currentPosition->Y += (currentScale * AKUMA_BACKJUMP_SPEED) * ((AKUMA_BACKJUMP_FRAMES - currentStateTime) / AKUMA_BACKJUMP_FRAMES);
		//	currentPosition->X -= (currentFaceLeft ? -1 : 1) * (currentScale * AKUMA_BACKJUMP_RSPEED);
		//}
		//else {
		//	currentPosition->Y -= (currentScale * AKUMA_BACKJUMP_SPEED) * ((currentStateTime - AKUMA_BACKJUMP_FRAMES) / AKUMA_BACKJUMP_FRAMES);
		//	currentPosition->X -= (currentFaceLeft ? -1 : 1) * (currentScale * AKUMA_BACKJUMP_RSPEED);
		//	if (currentStateTime == (int)(AKUMA_BACKJUMP_FRAMES * 0.9f))
		//	{
		//		currentAnimation = animJumpLand;
		//		currentAnimation->Start();
		//	}
		//	if (currentPosition->Y <= 0)
		//	{
		//		currentPosition->Y = 0;
		//		Fighter_SetState(Fighter::Idle);
		//	}
		//}
		break;
	case Fighter::Kick:
		//currentPosition->Y -= (currentScale * AKUMA_KICK_SPEED);
		//currentPosition->X -= (currentFaceLeft ? 1 : -1) * (currentScale * AKUMA_KICK_RSPEED);
		if (currentPosition->Y <= 0)
		{
			currentPosition->Y = 0;
			Fighter_SetState(Fighter::Idle);
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


	if (currentAnimation->HasEnded())
	{
		if (currentAnimation == animJumpTakeOff)
		{
			currentAnimation = animJumpFloat;
			currentAnimation->Start();
		}
		if (currentAnimation == animKOLand)
		{
			currentAnimation = animKO;
			currentAnimation->Start();
		}
	}
}

void Fighter::Fighter_SetState(int NewState)
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
	switch (NewState)
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


Vector2* Fighter::Fighter_GetPosition()
{
	return currentPosition;
}

void Fighter::Fighter_SetPosition(float X, float Y)
{
	currentPosition->X = X;
	currentPosition->Y = Y;
}

void Fighter::Fighter_SetPosition(Vector2* NewPosition)
{
	currentPosition->X = NewPosition->X;
	currentPosition->Y = NewPosition->Y;
}

void Fighter::Fighter_Render(int ScreenOffsetX, int ScreenOffsetY)
{
	// TODO: Write render code
}

bool Fighter::Fighter_IsFacingLeft()
{
	return currentFaceLeft;
}

void Fighter::Fighter_SetFacing(bool FacingLeft)
{
	if (currentState == Fighter::Idle)
	{
		currentFaceLeft = FacingLeft;
	}
}

void Fighter::Fighter_JumpPressed()
{
	switch( currentState )
	{
		case Fighter::Idle:
			Fighter_SetState( Fighter::Jump );
			break;
	}
}

void Fighter::Fighter_KickPressed()
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

void Fighter::Fighter_SuperPressed()
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

