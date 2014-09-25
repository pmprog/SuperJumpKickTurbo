
#pragma once

#include "../includes.h"

class SoundEffectsCache
{

	private:
		static std::vector<SoundEffectsCache*> cache;
		
		SoundEffectsCache( std::string File );


	public:
		std::string Filename;
		ALLEGRO_SAMPLE* Sample;
		int Locks;

		
		static ALLEGRO_SAMPLE* LoadSFX( std::string Filename );
		static void UnloadSFX( std::string Filename );
		static void UnloadSFX( ALLEGRO_SAMPLE* Bitmap );

};