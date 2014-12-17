
#include "roundover.h"

RoundOver::RoundOver(int PlayerWhoWon)
{
	winner = PlayerWhoWon;
}

void RoundOver::Begin()
{
	leaving = false;
	overbanner[0] = -50.0f;
	overbanner[1] = (DISPLAY->GetHeight() / 2) - 50.0f;
	overbanner[2] = -100.0f;
	overbanner[3] = (DISPLAY->GetHeight() / 2) + 50.0f;
	overbanner[4] = -50.0f;
	overbanner[5] = (DISPLAY->GetHeight() / 2) + 50.0f;
	overbanner[6] = 0.0f;
	overbanner[7] = (DISPLAY->GetHeight() / 2) - 50.0f;
}

void RoundOver::Pause()
{
}

void RoundOver::Resume()
{
}

void RoundOver::Finish()
{
}

void RoundOver::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		leaving = true;
	}
}

void RoundOver::Update()
{
	FRAMEWORK->ProgramStages->Previous()->Update();

	if( overbanner[4] < DISPLAY->GetWidth() )
	{
		overbanner[4] += 40.0f;
		overbanner[6] += 40.0f;
	}

	if( leaving )
	{
		if( overbanner[2] < DISPLAY->GetWidth() )
		{
			overbanner[0] += 40.0f;
			overbanner[2] += 40.0f;
		} else {
			delete FRAMEWORK->ProgramStages->Pop();
		}
	}
	
}

void RoundOver::Render()
{
	FRAMEWORK->ProgramStages->Previous()->Render();

	for( int i = 0; i < 8; i += 2 )
	{
		overbanner[i] += 12.0f;
		overbanner[i + 1] += 12.0f;
	}
	al_draw_filled_polygon( (const float*)&overbanner, 4, al_map_rgb( 0, 0, 0 ) );
	for( int i = 0; i < 8; i += 2 )
	{
		overbanner[i] -= 12.0f;
		overbanner[i + 1] -= 12.0f;
	}

	al_draw_filled_polygon( (const float*)&overbanner, 4, al_map_rgb( 255, 255, 0 ) );

	switch( winner )
	{
		case 0:
			// Draw
			break;
		case 1:
			// Player 1
			break;
		case 2:
			// Player 2
			break;
	}

}

bool RoundOver::IsTransition()
{
	return false;
}
