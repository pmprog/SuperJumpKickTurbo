
#include "roundover.h"

RoundOver::RoundOver(int PlayerWhoWon)
{
	winner = PlayerWhoWon;
	stagetime = 0;
}

void RoundOver::Begin()
{
	currentArena = (Arena*)FRAMEWORK->ProgramStages->Previous();

	fntTitle = al_load_ttf_font( "resources/titlefont.ttf", 48, 0 );
	fntTitleHeight = al_get_font_line_height( fntTitle );

	leaving = false;
	overbanner[0] = -50.0f;
	overbanner[1] = (DISPLAY->GetHeight() / 2) - 50.0f;
	overbanner[2] = -100.0f;
	overbanner[3] = overbanner[1] + 100.0f;
	overbanner[4] = -50.0f;
	overbanner[5] = overbanner[3];
	overbanner[6] = 0.0f;
	overbanner[7] = overbanner[1];

	switch( winner )
	{
		case 0:
			// Draw
			textDisplay = "Draw!";
			break;
		case 1:
			// Player 1
			textDisplay = "Player 1 Wins";
			break;
		case 2:
			// Player 2
			textDisplay = "Player 2 Wins";
			break;
	}

	textWidth = al_get_text_width( fntTitle, textDisplay.c_str() );
	textXposition = DISPLAY->GetWidth();


	overbannerXv = (DISPLAY->GetWidth() + 100) / ROUNDOVER_STEPSINOUT;
	textXv = ( textXposition - ((DISPLAY->GetWidth() - textWidth) / 2) ) / ROUNDOVER_STEPSINOUT;

	textXposition -= textXv;
	stagetime = 0;

}

void RoundOver::Pause()
{
}

void RoundOver::Resume()
{
}

void RoundOver::Finish()
{
	al_destroy_font( fntTitle );
	currentArena->ResetArena();
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
		overbanner[4] += overbannerXv;
		overbanner[6] += overbannerXv;
		textXposition -= textXv;
	}

	stagetime++;
	if( stagetime >= ROUNDOVER_STAGETIME )
	{
		leaving = true;
	}

	if( leaving )
	{
		if( overbanner[2] < DISPLAY->GetWidth() )
		{
			overbanner[0] += overbannerXv;
			overbanner[2] += overbannerXv;
			textXposition -= textXv;
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

	al_draw_text( fntTitle, al_map_rgb( 0, 0, 0 ), textXposition, (DISPLAY->GetHeight() - fntTitleHeight) / 2, 0, textDisplay.c_str() );

}

bool RoundOver::IsTransition()
{
	return false;
}
