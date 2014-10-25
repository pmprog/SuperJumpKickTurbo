
#include "fighter.h"

void Fighter::Initialise(std::string Config)
{
	ConfigFile* cfg = new ConfigFile( Config );
	std::string* tmpstring;

	CharacterName.clear();
	CharacterName.append( cfg->GetQuickStringValue("Name", "Unknown")->c_str() );

	tmpstring = cfg->GetQuickStringValue("Sprites", "");
	spriteSheet = new SpriteSheet( *tmpstring );
	delete tmpstring;

	// Add profile sprite
	spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Profile", 0, 0), cfg->GetQuickIntegerValue( "Profile", 1, 0), cfg->GetQuickIntegerValue( "Profile", 2, 0), cfg->GetQuickIntegerValue( "Profile", 3, 0) );

	// Add Idle Animation
	animIdle = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "IdleFrameTime", 20) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Idle" ) / 4; frameidx++ )
	{
		animIdle->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Idle", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 3, 0) ) );
	}

	// Add TakeOff Animation
	animJumpTakeOff = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "TakeOffFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "TakeOff" ) / 4; frameidx++ )
	{
		animJumpTakeOff->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 3, 0) ) );
	}

	// Add InAir Animation
	animJumpFloat = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "InAirFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "InAir" ) / 4; frameidx++ )
	{
		animJumpFloat->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "InAir", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 3, 0) ) );
	}

	// Add Land Animation
	animJumpLand = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "LandFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Land" ) / 4; frameidx++ )
	{
		animJumpLand->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Land", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 3, 0) ) );
	}

	// Add Kick Animation
	animKick = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "KickFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Kick" ) / 4; frameidx++ )
	{
		animKick->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Kick", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 3, 0) ) );
	}

	// Add Super Animation
	animSuper = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "SuperFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Super" ) / 4; frameidx++ )
	{
		animSuper->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Super", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Super", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Super", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Super", (frameidx * 4) + 3, 0) ) );
	}

	// Add KnockDown Animation
	animKnockDown = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "KnockDownFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "KnockDown" ) / 4; frameidx++ )
	{
		animKnockDown->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "KnockDown", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "KnockDown", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "KnockDown", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "KnockDown", (frameidx * 4) + 3, 0) ) );
	}

	// Add KnockDownLand Animation
	animKnockDownLand = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "KnockDownLandFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "KnockDownLand" ) / 4; frameidx++ )
	{
		animKnockDownLand->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "KnockDownLand", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "KnockDownLand", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "KnockDownLand", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "KnockDownLand", (frameidx * 4) + 3, 0) ) );
	}

	// Add KnockedOut Animation
	animKnockedOut = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "KnockedOutFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "KnockedOut" ) / 4; frameidx++ )
	{
		animKnockedOut->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "KnockedOut", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "KnockedOut", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "KnockedOut", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "KnockedOut", (frameidx * 4) + 3, 0) ) );
	}

	// Add Win Animation
	animWin = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "WinFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Win" ) / 4; frameidx++ )
	{
		animWin->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Win", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Win", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Win", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Win", (frameidx * 4) + 3, 0) ) );
	}

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
		if (currentAnimation == animKnockDownLand)
		{
			currentAnimation = animKnockedOut;
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

