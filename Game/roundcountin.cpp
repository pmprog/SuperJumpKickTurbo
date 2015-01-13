
#include "roundcountin.h"
#include "networkpacket.h"

void RoundCountIn::Begin()
{
	countdown = FRAMEWORK->GetFramesPerSecond() * 3;
	fntTitle = al_load_ttf_font( "resources/titlefont.ttf", 128, 0 );
	fntTitleHeight = al_get_font_line_height( fntTitle );

	overbanner[0] = 270;
	overbanner[1] = -100;
	overbanner[2] = 270;
	overbanner[3] = -50;
	overbanner[4] = 500;
	overbanner[5] = 0;
	overbanner[6] = 500;
	overbanner[7] = -50;

	bannerspeed = ((float)(DISPLAY->GetHeight() + 100) / (float)FRAMEWORK->GetFramesPerSecond()) * 3.0f;
	netping = false;

#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Stage: RoundCountIn::Begin()\n" );
#endif
}

void RoundCountIn::Pause()
{
}

void RoundCountIn::Resume()
{
}

void RoundCountIn::Finish()
{
	al_destroy_font( fntTitle );
#ifdef WRITE_LOG
	fprintf( FRAMEWORK->LogFile, "Stage: RoundCountIn::Finish()\n" );
#endif
}

void RoundCountIn::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
	}

	GamePacket netpacket;

	if( e->Type == EVENT_NETWORK_PACKET_RECEIVED )
	{
		if( e->Data.Network.Traffic.packet->dataLength != sizeof( netpacket ) )
		{
#ifdef WRITE_LOG
			fprintf( FRAMEWORK->LogFile, "Error: Invalid network packet length of %d, expecting %d", e->Data.Network.Traffic.packet->dataLength, sizeof( netpacket ) );
#endif
			delete FRAMEWORK->ProgramStages->Pop();
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
		memcpy((void*)&netpacket, e->Data.Network.Traffic.packet->data, e->Data.Network.Traffic.packet->dataLength );

		if( netpacket.Type == PACKET_TYPE_PING )
		{
			netping = true;
		}

		if( netpacket.Type == PACKET_TYPE_DISCONNECT )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

	}
}

void RoundCountIn::Update()
{
	if( Fighter::NetworkController != nullptr )
	{
		Fighter::NetworkController->Update();

		GamePacket netpacket;
		netpacket.Type = PACKET_TYPE_PING;
		Fighter::NetworkController->Send( (void*)&netpacket, sizeof(netpacket), true );

		if( !netping )
		{
			return;
		}
	}

	countdown--;
	if( countdown > FRAMEWORK->GetFramesPerSecond() * 2 )
	{
		overbanner[3] += bannerspeed;
		overbanner[5] += bannerspeed;
	}
	if( countdown <= (FRAMEWORK->GetFramesPerSecond() / 3.0f) )
	{
		overbanner[1] += bannerspeed;
		overbanner[7] += bannerspeed;
	}
	if( countdown <= 0 )
	{
		delete FRAMEWORK->ProgramStages->Pop();
		return;
	}
	Arena* a = (Arena*)FRAMEWORK->ProgramStages->Previous();
	a->Player1->Fighter_Update( true );
	a->Player2->Fighter_Update( true );
}

void RoundCountIn::Render()
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

	al_draw_filled_polygon( (const float*)&overbanner, 4, al_map_rgb( 128, 192, 64 ) );

	al_draw_textf( fntTitle, al_map_rgb( 0, 0, 0 ), 406, ((DISPLAY->GetHeight() - fntTitleHeight) / 2) + 6, ALLEGRO_ALIGN_CENTRE, "%d", (int)(countdown / FRAMEWORK->GetFramesPerSecond()) + 1 );
	al_draw_textf( fntTitle, al_map_rgb( 255, 255, 255 ), 400, (DISPLAY->GetHeight() - fntTitleHeight) / 2, ALLEGRO_ALIGN_CENTRE, "%d", (int)(countdown / FRAMEWORK->GetFramesPerSecond()) + 1 );
}

bool RoundCountIn::IsTransition()
{
	return false;
}
