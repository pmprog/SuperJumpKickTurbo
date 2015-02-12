
#include "matchover.h"

MatchOver::MatchOver(int WinnerIs)
{
	winner = WinnerIs;
}

void MatchOver::Begin()
{
	gameArena = (Arena*)FRAMEWORK->ProgramStages->Previous();

#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "********************************************* Match Over : Winner %d \n", winner );
	fprintf( FRAMEWORK->LogFile, "Stage: MatchOver::Begin()\n" );
#endif

	overbannerL[0] = 50;
	overbannerL[1] = -100;
	overbannerL[2] = 50;
	overbannerL[3] = -50;
	overbannerL[4] = 350;
	overbannerL[5] = 0;
	overbannerL[6] = 350;
	overbannerL[7] = -50;

	overbannerR[0] = 450;
	overbannerR[1] = -50;
	overbannerR[2] = 450;
	overbannerR[3] = 0;
	overbannerR[4] = 750;
	overbannerR[5] = -50;
	overbannerR[6] = 750;
	overbannerR[7] = -100;

	bannerspeed = (float)(DISPLAY->GetHeight() / (float)FRAMEWORK->GetFramesPerSecond()) * 3.0f;

	textstart = -380;

	fntStatus = al_load_ttf_font( "resources/titlefont.ttf", 40, 0 );
	fntMenu = al_load_ttf_font( "resources/titlefont.ttf", 24, 0 );

}

void MatchOver::Pause()
{
}

void MatchOver::Resume()
{
}

void MatchOver::Finish()
{
#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Stage: MatchOver::Finish()\n" );
#endif
	al_destroy_font( fntStatus );
	al_destroy_font( fntMenu );
}

void MatchOver::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		delete FRAMEWORK->ProgramStages->Pop();
		delete FRAMEWORK->ProgramStages->Pop();
	}
}

void MatchOver::Update()
{
	if( overbannerL[5] < 400 )
	{
		overbannerL[3] += bannerspeed;
		overbannerL[5] += bannerspeed;
		overbannerR[3] += bannerspeed;
		overbannerR[5] += bannerspeed;
		textstart += bannerspeed;
	}
}

void MatchOver::Render()
{
	gameArena->Render();

	for( int i = 0; i < 8; i += 2 )
	{
		overbannerL[i] += 12.0f;
		overbannerL[i + 1] += 12.0f;
	}
	al_draw_filled_polygon( (const float*)&overbannerL, 4, al_map_rgb( 0, 0, 0 ) );
	for( int i = 0; i < 8; i += 2 )
	{
		overbannerL[i] -= 12.0f;
		overbannerL[i + 1] -= 12.0f;
	}
	al_draw_filled_polygon( (const float*)&overbannerL, 4, al_map_rgb( 128, 192, 64 ) );

	for( int i = 0; i < 8; i += 2 )
	{
		overbannerR[i] += 12.0f;
		overbannerR[i + 1] += 12.0f;
	}
	al_draw_filled_polygon( (const float*)&overbannerR, 4, al_map_rgb( 0, 0, 0 ) );
	for( int i = 0; i < 8; i += 2 )
	{
		overbannerR[i] -= 12.0f;
		overbannerR[i + 1] -= 12.0f;
	}

	al_draw_filled_polygon( (const float*)&overbannerR, 4, al_map_rgb( 128, 192, 64 ) );

	char totalwins[10];
	gameArena->Player1TotalWins = (gameArena->Player1TotalWins % 1000000);
	gameArena->Player2TotalWins = (gameArena->Player2TotalWins % 1000000);

	DrawDropShadowText( fntStatus, al_map_rgb( 255, 255, 255 ), 200, textstart, ALLEGRO_ALIGN_CENTRE, ( winner == 1 ? "Winner" : "Loser" ) );
	DrawDropShadowText( fntMenu, al_map_rgb( 255, 255, 0 ), 200, textstart + 100, ALLEGRO_ALIGN_CENTRE, "Total Wins" );
	sprintf( (char*)&totalwins, "%d", gameArena->Player1TotalWins );
	DrawDropShadowText( fntMenu, al_map_rgb( 255, 255, 0 ), 200, textstart + 140, ALLEGRO_ALIGN_CENTRE, (char*)&totalwins );

	DrawDropShadowText( fntStatus, al_map_rgb( 255, 255, 255 ), 600, textstart, ALLEGRO_ALIGN_CENTRE, ( winner == 2 ? "Winner" : "Loser" ) );
	DrawDropShadowText( fntMenu, al_map_rgb( 255, 255, 0 ), 600, textstart + 100, ALLEGRO_ALIGN_CENTRE, "Total Wins" );
	sprintf( (char*)&totalwins, "%d", gameArena->Player2TotalWins );
	DrawDropShadowText( fntMenu, al_map_rgb( 255, 255, 0 ), 600, textstart + 140, ALLEGRO_ALIGN_CENTRE, (char*)&totalwins );

}

bool MatchOver::IsTransition()
{
	return false;
}

void MatchOver::DrawDropShadowText( ALLEGRO_FONT* Font, ALLEGRO_COLOR Colour, float X, float Y, int Flags, const char* Text )
{
	al_draw_text( Font, al_map_rgb( 0, 0, 0 ), X + 3, Y + 3, Flags, Text );
	al_draw_text( Font, Colour, X, Y, Flags, Text );
}
