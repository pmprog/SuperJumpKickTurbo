
#include "fighter.h"
#include "arena.h"

Network* Fighter::NetworkController = nullptr;

Fighter::Fighter( FighterController Controls, std::string Config, Arena* FightArena, bool AlternativeSprites )
{

#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Fighter: Constructing %s\n", Config.c_str() );
#endif

	ConfigFile* cfg = new ConfigFile( Config );
	std::string* tmpstring;

	Controller = Controls;

	currentArena = FightArena;

	currentPosition = new Vector2();

	CharacterName.clear();
	CharacterName.append( cfg->GetQuickStringValue("Name", "Unknown")->c_str() );

	if( !AlternativeSprites )
	{
		tmpstring = cfg->GetQuickStringValue("Sprites", "");
	} else {
		tmpstring = cfg->GetQuickStringValue("SpritesAlt", "");
	}
	spriteSheet = new SpriteSheet( *tmpstring );
	delete tmpstring;

	jumpFrames = cfg->GetQuickFloatValue( "JumpFrames", 60.0f );
	jumpSpeed = cfg->GetQuickFloatValue( "JumpSpeed", 3.0f );
	jumpBackFrames = cfg->GetQuickFloatValue( "JumpBackFrames", 30.0f );
	jumpBackVSpeed = cfg->GetQuickFloatValue( "JumpBackVSpeed", 2.0f );
	jumpBackHSpeed = cfg->GetQuickFloatValue( "JumpBackHSpeed", 1.0f );
	kickVSpeed = cfg->GetQuickFloatValue( "KickVSpeed", 2.0f );
	kickHSpeed = cfg->GetQuickFloatValue( "KickHSpeed", 1.5f );

	// Add profile sprite
	spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Profile", 0, 0), cfg->GetQuickIntegerValue( "Profile", 1, 0), cfg->GetQuickIntegerValue( "Profile", 2, 0), cfg->GetQuickIntegerValue( "Profile", 3, 0) );

	// Add Idle Animation
	animIdle = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "IdleFrameTime", 20) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Idle" ) / 4; frameidx++ )
	{
		animIdle->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Idle", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Idle", (frameidx * 4) + 3, 0) ) );
		collisionIdle.push_back( new Box( cfg->GetQuickIntegerValue( "IdleHit", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "IdleHit", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "IdleHit", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "IdleHit", (frameidx * 4) + 3, 0) ) );
	}

	// Add TakeOff Animation
	animJumpTakeOff = new Animation( spriteSheet, false, cfg->GetQuickIntegerValue( "TakeOffFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "TakeOff" ) / 4; frameidx++ )
	{
		animJumpTakeOff->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "TakeOff", (frameidx * 4) + 3, 0) ) );
		collisionJumpTakeOff.push_back( new Box( cfg->GetQuickIntegerValue( "TakeOffHit", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "TakeOffHit", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "TakeOffHit", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "TakeOffHit", (frameidx * 4) + 3, 0) ) );
	}

	// Add InAir Animation
	animJumpFloat = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "InAirFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "InAir" ) / 4; frameidx++ )
	{
		animJumpFloat->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "InAir", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "InAir", (frameidx * 4) + 3, 0) ) );
		collisionJumpFloat.push_back( new Box( cfg->GetQuickIntegerValue( "InAirHit", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "InAirHit", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "InAirHit", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "InAirHit", (frameidx * 4) + 3, 0) ) );
	}

	// Add Land Animation
	animJumpLand = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "LandFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Land" ) / 4; frameidx++ )
	{
		animJumpLand->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Land", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Land", (frameidx * 4) + 3, 0) ) );
		collisionJumpLand.push_back( new Box( cfg->GetQuickIntegerValue( "LandHit", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "LandHit", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "LandHit", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "LandHit", (frameidx * 4) + 3, 0) ) );
	}

	// Add Kick Animation
	animKick = new Animation( spriteSheet, true, cfg->GetQuickIntegerValue( "KickFrameTime", 40) );
	for( int frameidx = 0; frameidx < cfg->GetArraySize( "Kick" ) / 4; frameidx++ )
	{
		animKick->AddFrame( spriteSheet->AddSprite( cfg->GetQuickIntegerValue( "Kick", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "Kick", (frameidx * 4) + 3, 0) ) );
		collisionKick.push_back( new Box( cfg->GetQuickIntegerValue( "KickHit", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "KickHit", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "KickHit", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "KickHit", (frameidx * 4) + 3, 0) ) );
		attackKick.push_back( new Box( cfg->GetQuickIntegerValue( "KickAttack", (frameidx * 4), 0), cfg->GetQuickIntegerValue( "KickAttack", (frameidx * 4) + 1, 0), cfg->GetQuickIntegerValue( "KickAttack", (frameidx * 4) + 2, 0), cfg->GetQuickIntegerValue( "KickAttack", (frameidx * 4) + 3, 0) ) );
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
	animKnockDownLand = new Animation( spriteSheet, false, cfg->GetQuickIntegerValue( "KnockDownLandFrameTime", 40) );
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

	jumpIsPressed = false;
	kickIsPressed = false;

	Fighter_SetState( Fighter::FighterStates::Idle );
	FighterHit = false;

	State_Clear();

}

void Fighter::CharSelect_RenderProfileIcon(int ScreenX, int ScreenY)
{
	spriteSheet->DrawSprite( 0, ScreenX - (spriteSheet->GetFrame(0)->Width / 2), ScreenY - (spriteSheet->GetFrame(0)->Height / 2) );
}

void Fighter::Fighter_Update( bool IgnoreCollisions )
{
	Fighter* opponent;
	Box* collisionarea;
	Box* kickarea;

	// Only process AI if checking for collisions (ie, playing the game)
	if( !IgnoreCollisions )
	{
		if( Controller == FighterController::CPU_Easy )
		{
			AI_Update( 0 );
		}
		if( Controller == FighterController::CPU_Medium )
		{
			AI_Update( 1 );
		}
		if( Controller == FighterController::CPU_Hard )
		{
			AI_Update( 2 );
		}
	}

	if( jumpIsPressed )
	{
		switch( currentState )
		{
			case Fighter::Idle:
				Fighter_SetState( Fighter::Jump );
				break;
		}
		jumpIsPressed = false;
	}

	if( kickIsPressed )
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
		kickIsPressed = false;
	}


	currentAnimation->Update();
	currentStateTime++;

	switch (currentState)
	{
	case Fighter::Idle:
		break;
	case Fighter::Jump:
		if (currentStateTime < jumpFrames)
		{
			currentPosition->Y += jumpSpeed * ((jumpFrames - currentStateTime) / jumpFrames);
		}
		else {
			currentPosition->Y -= jumpSpeed * ((currentStateTime - jumpFrames) / jumpFrames);
			if (currentStateTime == (int)(jumpFrames * 0.9f))
			{
				currentAnimation = animJumpLand;
				currentAnimation->Start();
			}
			if (currentPosition->Y <= 0)
			{
				currentPosition->Y = 0;
				Fighter_SetState(Fighter::Idle);
			}
		}
		break;
	case Fighter::BackJump:
		if (currentStateTime < jumpBackFrames)
		{
			currentPosition->Y += jumpBackVSpeed * ((jumpBackFrames - currentStateTime) / jumpBackFrames);
			currentPosition->X -= (currentFaceLeft ? -1 : 1) * jumpBackHSpeed;
		}
		else {
			currentPosition->Y -= jumpBackVSpeed * ((currentStateTime - jumpBackFrames) / jumpBackFrames);
			currentPosition->X -= (currentFaceLeft ? -1 : 1) * jumpBackHSpeed;
			if (currentStateTime == (int)(jumpBackFrames * 0.9f))
			{
				currentAnimation = animJumpLand;
				currentAnimation->Start();
			}
			if (currentPosition->Y <= 0)
			{
				currentPosition->Y = 0;
				Fighter_SetState(Fighter::Idle);
			}
		}
		break;
	case Fighter::Kick:
		currentPosition->Y -= kickVSpeed;
		currentPosition->X -= (currentFaceLeft ? 1 : -1) * kickHSpeed;
		if (currentPosition->Y <= 0)
		{
			currentPosition->Y = 0;
			Fighter_SetState(Fighter::Idle);
		}
		if( currentArena != 0 && !IgnoreCollisions )
		{
			opponent = currentArena->GetOpponent( this );
			collisionarea = opponent->Fighter_GetCurrentHitBox();
			if( collisionarea != 0 )
			{
				kickarea = CollisionBoxToScreenBox( attackKick.at( currentAnimation->GetCurrentFrame() ) );
				if( kickarea->Collides( collisionarea ) )
				{
					opponent->FighterHit = true;
					currentArena->AddCollisionAt( kickarea->GetCentre() );
				}
				delete kickarea;
			}
			delete collisionarea;
		}
		break;
	case Fighter::Super:
		break;

	case Fighter::Knockdown:
		FighterHit = false;
		currentPosition->Y -= jumpSpeed;
		if (currentPosition->Y <= 0)
		{
			currentPosition->Y = 0;
			if( currentAnimation != animKnockDownLand )
			{
				AUDIO->PlaySoundEffect( "resources/collision.wav" );
				currentAnimation = animKnockDownLand;
				currentAnimation->Start();
			}
		}
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
			Fighter_SetState( Fighter::Loser );
		}
	}

	if( currentPosition->X < 0 )
	{
		currentPosition->X = 0;
	}
	if( currentArena != 0 && currentPosition->X > currentArena->ArenaWidth )
	{
		currentPosition->X = currentArena->ArenaWidth;
	}

}

Fighter::FighterStates Fighter::Fighter_GetState()
{
	return currentState;
}

void Fighter::Fighter_SetState(FighterStates NewState)
{
	Fighter_SetState( NewState, true );
}

void Fighter::Fighter_SetState( FighterStates NewState, bool SaveState )
{
	// Exit State Code

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
		AUDIO->PlaySoundEffect( "resources/jump.wav" );
		currentAnimation = animJumpTakeOff;
		currentAnimation->Start();
		break;
	case Fighter::Kick:
		AUDIO->PlaySoundEffect( "resources/jump.wav" );
		currentAnimation = animKick;
		currentAnimation->Start();
		break;
	case Fighter::Super:
		currentAnimation = animSuper;
		currentAnimation->Start();
		break;
	case Fighter::Knockdown:
		AUDIO->PlaySoundEffect( "resources/collision.wav" );
		currentAnimation = animKnockDown;
		currentAnimation->Start();
		break;
	case Fighter::Victor:
		currentAnimation = animWin;
		currentAnimation->Start();
		break;
	case Fighter::Loser:
		currentAnimation = animKnockedOut;
		currentAnimation->Start();
		break;
	}

	if( SaveState && currentArena != nullptr )
	{
		State_Save( currentArena->RoundFrameCount );
	}
}

int Fighter::Fighter_GetStateTime()
{
	return currentStateTime;
}

void Fighter::Fighter_SetStateTime( int NewStateTime )
{
	currentStateTime = NewStateTime;
}

Vector2* Fighter::Fighter_GetPosition()
{
	return currentPosition;
}

void Fighter::Fighter_SetPosition(float X, float Y)
{
	currentPosition->X = X;
	currentPosition->Y = Y;

	if( currentArena != nullptr )
	{
		State_Save( currentArena->RoundFrameCount );
	}

}

void Fighter::Fighter_SetPosition(Vector2* NewPosition)
{
	currentPosition->X = NewPosition->X;
	currentPosition->Y = NewPosition->Y;

	if( currentArena != nullptr )
	{
		State_Save( currentArena->RoundFrameCount );
	}
}

void Fighter::Fighter_Render(int ScreenOffsetX, int ScreenOffsetY)
{
	int screenY = 432 - currentPosition->Y + ScreenOffsetY - spriteSheet->GetFrame( currentAnimation->GetCurrentFramesSpriteIndex() )->Height;
	int screenX = currentPosition->X - ScreenOffsetX;
	screenX -= (currentFaceLeft ? -1 : 1) * (spriteSheet->GetFrame( currentAnimation->GetCurrentFramesSpriteIndex() )->Width / 2);
	currentAnimation->DrawFrame( screenX, screenY, currentFaceLeft, false );

	if( FRAMEWORK->Settings->GetQuickBooleanValue( "Debug.ShowCollisionBoxes", false ) )
	{
		Box* collisionarea = Fighter_GetCurrentHitBox();
		if( collisionarea != 0 )
		{
			al_draw_filled_rectangle( collisionarea->GetLeft() - ScreenOffsetX, collisionarea->GetTop() + ScreenOffsetY, collisionarea->GetRight() - ScreenOffsetX, collisionarea->GetBottom() + ScreenOffsetY, al_map_rgba( 255, 0, 0, 128 ) );
		}
		delete collisionarea;
		if( currentState == Fighter::Kick )
		{
			Box* kickarea = CollisionBoxToScreenBox( attackKick.at( currentAnimation->GetCurrentFrame() ) );
			if( kickarea != 0 )
			{
				al_draw_filled_rectangle( kickarea->GetLeft() - ScreenOffsetX, kickarea->GetTop() + ScreenOffsetY, kickarea->GetRight() - ScreenOffsetX, kickarea->GetBottom() + ScreenOffsetY, al_map_rgba( 0, 0, 255, 128 ) );
			}
			delete kickarea;
		}
	}

}

bool Fighter::Fighter_IsFacingLeft()
{
	return currentFaceLeft;
}

void Fighter::Fighter_SetFacing(bool FacingLeft)
{
	if (currentState == Fighter::Idle)
	{
		if( currentFaceLeft != FacingLeft )
		{
			currentFaceLeft = FacingLeft;
			if( currentArena != nullptr )
			{
				State_Save( currentArena->RoundFrameCount );
			}
		}
	}
}

void Fighter::Fighter_JumpPressed()
{
	jumpIsPressed = true;
}

void Fighter::Fighter_KickPressed()
{
	kickIsPressed = true;
}

void Fighter::Fighter_SuperPressed()
{
	switch( currentState )
	{
		case Fighter::Idle:
		case Fighter::Jump:
		case Fighter::BackJump:
		case Fighter::Kick:
			// Fighter_SetState( Fighter::Super );
			break;
	}
}

Box* Fighter::Fighter_GetCurrentHitBox()
{
	std::vector<Box*>* boxlist = 0;
	if( currentAnimation == animIdle )
	{
		boxlist = &collisionIdle;
	}
	if( currentAnimation == animJumpTakeOff )
	{
		boxlist = &collisionJumpTakeOff;
	}
	if( currentAnimation == animJumpFloat )
	{
		boxlist = &collisionJumpFloat;
	}
	if( currentAnimation == animJumpLand )
	{
		boxlist = &collisionJumpLand;
	}
	if( currentAnimation == animKick )
	{
		boxlist = &collisionKick;
	}
	if( boxlist == 0 )
	{
		return 0;
	}

	return CollisionBoxToScreenBox(boxlist->at( currentAnimation->GetCurrentFrame() ));
}

Box* Fighter::CollisionBoxToScreenBox(Box* Source)
{
	int screenY = 432 - currentPosition->Y - spriteSheet->GetFrame( currentAnimation->GetCurrentFramesSpriteIndex() )->Height;
	int screenX = currentPosition->X;

	if( currentFaceLeft )
	{
		screenX += (spriteSheet->GetFrame( currentAnimation->GetCurrentFramesSpriteIndex() )->Width / 2);
		screenX -= Source->GetRight();
	} else {
		screenX -= (spriteSheet->GetFrame( currentAnimation->GetCurrentFramesSpriteIndex() )->Width / 2);
		screenX += Source->GetLeft();
	}
	screenY += Source->GetTop();

	Box* b = new Box( screenX, screenY, Source->GetWidth(), Source->GetHeight()  );
	return b;
}

void Fighter::AI_Update( int Skill )
{
	int randomkeys = rand() % 30;
	
	switch( randomkeys )
	{
		case 0:
			Fighter_JumpPressed();
			break;
		case 1:
			Fighter_KickPressed();
			break;
	}
}

void Fighter::State_Clear()
{
#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Fighter State: Clear %s\n", PlayerName.c_str() );
#endif
	// Clear all rollback states
	memset( (void*)&RollbackStates, -1, sizeof(FighterSaveState) * FIGHTER_MAXIMUM_ROLLBACK_STATES );
}

void Fighter::State_Save(uint64_t FrameCount)
{
#ifdef WRITE_LOG
			fprintf( FRAMEWORK->LogFile, "Fighter State: Save %s at Frame %d \n", PlayerName.c_str(), FrameCount );
#endif

	FighterSaveState tempstates[FIGHTER_MAXIMUM_ROLLBACK_STATES - 1];

	// Bulk copy states back one
	memcpy( (void*)&tempstates, (void*)&RollbackStates, sizeof(FighterSaveState) * (FIGHTER_MAXIMUM_ROLLBACK_STATES - 1) );
	memcpy( (void*)&RollbackStates[1], (void*)&tempstates, sizeof(FighterSaveState) * (FIGHTER_MAXIMUM_ROLLBACK_STATES - 1) );

	RollbackStates[0].FrameCount = FrameCount;
	RollbackStates[0].State = currentState;
	RollbackStates[0].StateTime = currentStateTime;
	RollbackStates[0].FaceLeft = currentFaceLeft;
	RollbackStates[0].X = currentPosition->X;
	RollbackStates[0].Y = currentPosition->Y;
	RollbackStates[0].BeenHit = FighterHit;
}

bool Fighter::State_Load(uint64_t FrameCount)
{
#ifdef WRITE_LOG
			fprintf( FRAMEWORK->LogFile, "Fighter State: Load %s at Frame %d \n", PlayerName.c_str(), FrameCount );
#endif

	FighterSaveState tempstates[FIGHTER_MAXIMUM_ROLLBACK_STATES];

	bool foundState = false;
	for( int i = 0; i < FIGHTER_MAXIMUM_ROLLBACK_STATES; i++ )
	{
		if( RollbackStates[i].FrameCount >= 0 && RollbackStates[i].FrameCount <= FrameCount )
		{
			Fighter_SetState( RollbackStates[i].State, false );
			currentStateTime = RollbackStates[i].StateTime;
			currentFaceLeft = RollbackStates[i].FaceLeft;
			currentPosition->X = RollbackStates[i].X;
			currentPosition->Y = RollbackStates[i].Y;
			FighterHit = RollbackStates[i].BeenHit;

			for( long l = RollbackStates[i].FrameCount; l <= FrameCount; l++ )
			{
				Fighter_Update( true );
			}

#ifdef WRITE_LOG
			fprintf( FRAMEWORK->LogFile, "  Player X : %d \n", currentPosition->X );
			fprintf( FRAMEWORK->LogFile, "  Player Y : %d \n", currentPosition->Y );
			fprintf( FRAMEWORK->LogFile, "  Player State : %d \n", currentState );
			fprintf( FRAMEWORK->LogFile, "  Player StateTime : %d \n", currentStateTime );
			fprintf( FRAMEWORK->LogFile, "  Player Hit : %d \n", FighterHit );
			fprintf( FRAMEWORK->LogFile, "  Player Face : %d \n", currentFaceLeft );
#endif

			// Bulk copy states to hide rolled back future
			memcpy( (void*)&tempstates, (void*)&RollbackStates[i], sizeof(FighterSaveState) * (FIGHTER_MAXIMUM_ROLLBACK_STATES - i) );
			memcpy( (void*)&RollbackStates[0], (void*)&tempstates, sizeof(FighterSaveState) * (FIGHTER_MAXIMUM_ROLLBACK_STATES - i) );


			foundState = true;
			break;
		}
	}
	return foundState;
}

void Fighter::State_Inject(uint64_t FrameCount, FighterSaveState* NewState)
{
#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Fighter State: Inject %s at Frame %d \n", PlayerName.c_str(), FrameCount );
	fprintf( FRAMEWORK->LogFile, "  Player X : %d \n", NewState->X );
	fprintf( FRAMEWORK->LogFile, "  Player Y : %d \n", NewState->Y );
	fprintf( FRAMEWORK->LogFile, "  Player State : %d \n", NewState->State );
	fprintf( FRAMEWORK->LogFile, "  Player StateTime : %d \n", NewState->StateTime );
	fprintf( FRAMEWORK->LogFile, "  Player Hit : %d \n", NewState->BeenHit );
	fprintf( FRAMEWORK->LogFile, "  Player Face : %d \n", NewState->FaceLeft );
#endif

	State_Load( FrameCount - 1 );
	Fighter_SetState( NewState->State, false );
	currentStateTime = NewState->StateTime;
	currentFaceLeft = NewState->FaceLeft;
	currentPosition->X = NewState->X;
	currentPosition->Y = NewState->Y;
	FighterHit = NewState->BeenHit;
	State_Save( FrameCount );
}

Fighter::FighterSaveState* Fighter::State_GetCurrent(uint64_t FrameCount)
{
	Fighter::FighterSaveState* current = (FighterSaveState*)malloc( sizeof(FighterSaveState) );

#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Fighter State: Get %s at Frame %d \n", PlayerName.c_str(), FrameCount );
	fprintf( FRAMEWORK->LogFile, "  Player X : %d \n", currentPosition->X );
	fprintf( FRAMEWORK->LogFile, "  Player Y : %d \n", currentPosition->Y );
	fprintf( FRAMEWORK->LogFile, "  Player State : %d \n", currentState );
	fprintf( FRAMEWORK->LogFile, "  Player StateTime : %d \n", currentStateTime );
	fprintf( FRAMEWORK->LogFile, "  Player Hit : %d \n", FighterHit );
	fprintf( FRAMEWORK->LogFile, "  Player Face : %d \n", currentFaceLeft );
#endif

	current->FrameCount = FrameCount;
	current->State = currentState;
	current->StateTime = currentStateTime;
	current->FaceLeft = currentFaceLeft;
	current->X = currentPosition->X;
	current->Y = currentPosition->Y;
	current->BeenHit = FighterHit;

	return current;
}
