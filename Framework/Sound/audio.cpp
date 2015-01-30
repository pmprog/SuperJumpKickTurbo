
#include "audio.h"
#include "../framework.h"

Audio* Audio::CurrentAudio = nullptr;


Audio::Audio()
{
#ifdef WRITE_LOG
  fprintf( FRAMEWORK->LogFile, "Framework: Initialise Audio\n" );
#endif

	if( !al_install_audio() )
	{
		fprintf( FRAMEWORK->LogFile, "Audio_Initialise: Failed to install audio\n" );
		return;
	}
	if( !al_init_acodec_addon() )
	{
		fprintf( FRAMEWORK->LogFile, "Audio_Initialise: Failed to install codecs\n" );
		return;
	}

	// Allow playing samples
	al_reserve_samples( 10 );

	audioVoice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	if( audioVoice == 0 )
	{
		fprintf( FRAMEWORK->LogFile, "Audio_Initialise: Failed to create voice\n" );
		return;
	}
	audioMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	if( audioMixer == 0 )
	{
		fprintf( FRAMEWORK->LogFile, "Audio_Initialise: Failed to create mixer\n" );
		al_destroy_voice( audioVoice );
		audioVoice = 0;
		return;
	}
	if( !al_attach_mixer_to_voice( audioMixer, audioVoice ) )
	{
		fprintf( FRAMEWORK->LogFile, "Audio_Initialise: Failed to attach mixer to voice\n" );
		al_destroy_voice( audioVoice );
		audioVoice = 0;
		al_destroy_mixer( audioMixer );
		audioMixer = 0;
		return;
	}

	musicStream = nullptr;
}

Audio::~Audio()
{
#ifdef WRITE_LOG
  fprintf( FRAMEWORK->LogFile, "Framework: Shutdown Audio\n" );
#endif
	if( audioVoice != 0 )
	{
		al_destroy_voice( audioVoice );
		audioVoice = 0;
	}
	if( audioMixer != 0 )
	{
		al_destroy_mixer( audioMixer );
		audioMixer = 0;
	}
	al_uninstall_audio();
}

void Audio::PlayMusic( std::string Filename, bool Loop )
{
	if( audioVoice == 0 || audioMixer == 0 )
	{
		return;
	}

	if( musicStream != nullptr )
	{
		StopMusic();
	}

	// Only play if Music is set
	if( FRAMEWORK->Settings->GetQuickBooleanValue( "Audio.Music", true ) )
	{

#ifdef WRITE_LOG
		fprintf( FRAMEWORK->LogFile, "Framework: Start audio file %s\n", Filename.c_str() );
#endif
		musicStream = al_load_audio_stream( Filename.c_str(), 4, 2048 );
		if( musicStream != nullptr )
		{
			al_attach_audio_stream_to_mixer( musicStream, audioMixer );
			al_set_audio_stream_playmode( musicStream, ( Loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE ) );
			FRAMEWORK->RegisterEventSource( al_get_audio_stream_event_source( musicStream ) );
			al_set_audio_stream_playing( musicStream, true );
		} else {
#ifdef WRITE_LOG
			fprintf( FRAMEWORK->LogFile, "Framework: Could not load music '%s'\n", Filename.c_str() );
#endif
		}

	}
}

void Audio::StopMusic()
{
	if( audioVoice == 0 || audioMixer == 0 )
	{
		return;
	}

#ifdef WRITE_LOG
  fprintf( FRAMEWORK->LogFile, "Framework: Stop audio\n" );
#endif
	if( musicStream != nullptr )
	{
		FRAMEWORK->UnregisterEventSource( al_get_audio_stream_event_source( musicStream ) );
		al_set_audio_stream_playing( musicStream, false );
		al_detach_audio_stream( musicStream );
		// Causes game to hang!?
		// al_destroy_audio_stream( musicStream );
		musicStream = nullptr;
	}
}

ALLEGRO_MIXER* Audio::GetMixer()
{
	return audioMixer;
}

void Audio::PlaySoundEffect( std::string Filename )
{
	// Only play if Music is set
	if( FRAMEWORK->Settings->GetQuickBooleanValue( "Audio.Sound", true ) )
	{
		al_play_sample( SoundEffectsCache::LoadSFX( Filename ), 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr );
	}
}
