
#include "playerselect.h"
#include "arena.h"

PlayerSelect::PlayerSelect( Fighter::FighterController Player1Controls, Fighter::FighterController Player2Controls )
{
	datalists = new ConfigFile( "resources/data.txt" );

	p1charidx = 0;
	p1active = (Player1Controls != Fighter::FighterController::NoControls);
	if( p1active )
	{
		ConstructPlayer1( Player1Controls, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
	} else {
		p1char = 0;
	}

	p2charidx = 0;
	p2active = (Player2Controls != Fighter::FighterController::NoControls);
	if( p2active )
	{
		ConstructPlayer2( Player2Controls, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
	} else {
		p2char = 0;
	}

}

void PlayerSelect::Begin()
{
}

void PlayerSelect::Pause()
{
}

void PlayerSelect::Resume()
{
}

void PlayerSelect::Finish()
{
}

void PlayerSelect::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Jump", ALLEGRO_KEY_LSHIFT ) )
		{
			if( p1active )
			{
				p1charidx = (p1charidx + 1) % datalists->GetArraySize( "Characters" );
				p1active = true;
			}
			ConstructPlayer1( Fighter::FighterController::LocalKeyboardP1, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player1.Kick", ALLEGRO_KEY_LCTRL ) )
		{
			if( !p1active )
			{
				ConstructPlayer1( Fighter::FighterController::LocalKeyboardP1, *datalists->GetQuickStringValue( "Characters", p1charidx, "" ) );
				p1active = true;
			} else {
				p1char->Fighter_SetState( Fighter::FighterStates::Victor );
			}
		}

		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Jump", ALLEGRO_KEY_RSHIFT ) )
		{
			if( p2active )
			{
				p2charidx = (p2charidx + 1) % datalists->GetArraySize( "Characters" );
			}
			ConstructPlayer2( Fighter::FighterController::LocalKeyboardP2, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
			p2active = true;
		}
		if( e->Data.Keyboard.KeyCode == FRAMEWORK->Settings->GetQuickIntegerValue( "Player2.Kick", ALLEGRO_KEY_RCTRL ) )
		{
			if( !p2active )
			{
				ConstructPlayer2( Fighter::FighterController::LocalKeyboardP2, *datalists->GetQuickStringValue( "Characters", p2charidx, "" ) );
				p2active = true;
			} else {
				p2char->Fighter_SetState( Fighter::FighterStates::Victor );
			}
		}
	}
}

void PlayerSelect::Update()
{
	Arena* ingame;

	if( p1char != 0 )
	{
		p1char->Fighter_Update( true );
	}
	if( p2char != 0 )
	{
		p2char->Fighter_Update( true );
	}

	if( p1active && p2active )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor && p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Both players have selected their fighters
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
		}
	}

	if( p1active && !p2active )
	{
		if( p1char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer2( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
		}
	}

	if( !p1active && p2active )
	{
		if( p2char->Fighter_GetState() == Fighter::FighterStates::Victor )
		{
			// Start up single player game
			ConstructPlayer1( Fighter::FighterController::CPU_Easy, *datalists->GetQuickStringValue( "Characters", rand() % datalists->GetArraySize( "Characters" ), "" ) );
			ingame =  new Arena( "resources/japan.png", p1char, p2char );
			p1char->currentArena = ingame;
			p2char->currentArena = ingame;
			delete FRAMEWORK->ProgramStages->Pop();
			FRAMEWORK->ProgramStages->Push( ingame );
		}
	}

}

void PlayerSelect::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	if( p1char != 0 )
	{
		p1char->Fighter_Render( 0, 0 );
	}

	if( p2char != 0 )
	{
		p2char->Fighter_Render( 0, 0 );
	}
}

bool PlayerSelect::IsTransition()
{
	return false;
}

void PlayerSelect::ConstructPlayer1( Fighter::FighterController Controller, std::string Character )
{
	p1char = new Fighter( Controller, Character.c_str(), 0, false );
	p1char->Fighter_SetPosition( DISPLAY->GetWidth() / 4, 0 );
	p1char->Fighter_SetState( Fighter::Idle );
	p1char->Fighter_SetFacing( false );
}

void PlayerSelect::ConstructPlayer2( Fighter::FighterController Controller, std::string Character )
{
	p2char = new Fighter( Controller, Character.c_str(), 0, true );
	p2char->Fighter_SetPosition( (DISPLAY->GetWidth() / 4) * 3, 0 );
	p2char->Fighter_SetState( Fighter::Idle );
	p2char->Fighter_SetFacing( true );
}

