
#include "matchover.h"

void MatchOver::Begin()
{
#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Stage: MatchOver::Begin()\n" );
#endif
	gameArena = (Arena*)FRAMEWORK->ProgramStages->Previous();

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
}

bool MatchOver::IsTransition()
{
	return false;
}
