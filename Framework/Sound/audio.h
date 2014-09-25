
#pragma once

#include "../includes.h"
#include "sfxcache.h"

#define AUDIO	Audio::CurrentAudio

class Audio
{

	private:
		ALLEGRO_MIXER* audioMixer;
		ALLEGRO_VOICE* audioVoice;

		ALLEGRO_AUDIO_STREAM* musicStream;

	public:
		static Audio* CurrentAudio;

    Audio();
    ~Audio();

    void PlayMusic( std::string Filename, bool Loop );
    void StopMusic();

		void PlaySoundEffect( std::string Filename );

		ALLEGRO_MIXER* GetMixer();
};

