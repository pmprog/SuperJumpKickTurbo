
#include "sfxcache.h"

std::vector<SoundEffectsCache*> SoundEffectsCache::cache;

SoundEffectsCache::SoundEffectsCache( std::string File )
{
	Filename = File;
	Sample = al_load_sample( File.c_str() );
	Locks = 0;
}

ALLEGRO_SAMPLE* SoundEffectsCache::LoadSFX( std::string Filename )
{
	SoundEffectsCache* b;
	for( std::vector<SoundEffectsCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (SoundEffectsCache*)*i;
		if( b->Filename == Filename )
		{
			b->Locks++;
			return b->Sample;
		}
	}

	b = new SoundEffectsCache( Filename );
	cache.push_back( b );
	return b->Sample;
}

void SoundEffectsCache::UnloadSFX( std::string Filename )
{
	SoundEffectsCache* b;
	for( std::vector<SoundEffectsCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (SoundEffectsCache*)*i;
		if( b->Filename == Filename )
		{
			b->Locks--;
			if( b->Locks <= 0 )
			{
				al_destroy_sample( b->Sample );
				cache.erase( i );
				return;
			}
		}
	}
}

void SoundEffectsCache::UnloadSFX( ALLEGRO_SAMPLE* Bitmap )
{
	SoundEffectsCache* b;
	for( std::vector<SoundEffectsCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (SoundEffectsCache*)*i;
		if( b->Sample == Bitmap )
		{
			b->Locks--;
			if( b->Locks <= 0 )
			{
				al_destroy_sample( b->Sample );
				cache.erase( i );
				return;
			}
		}
	}
}
