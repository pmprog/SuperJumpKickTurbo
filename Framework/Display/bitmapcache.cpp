
#include "bitmapcache.h"

std::vector<BitmapCache*> BitmapCache::cache;

BitmapCache::BitmapCache( std::string File )
{
	Filename = File;
	Image = al_load_bitmap( File.c_str() );
	Locks = 0;
}

ALLEGRO_BITMAP* BitmapCache::LoadBitmap( std::string Filename )
{
	BitmapCache* b;
	for( std::vector<BitmapCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (BitmapCache*)*i;
		if( b->Filename == Filename )
		{
			b->Locks++;
			return b->Image;
		}
	}

	b = new BitmapCache( Filename );
	cache.push_back( b );
	return b->Image;
}

void BitmapCache::UnloadBitmap( std::string Filename )
{
	BitmapCache* b;
	for( std::vector<BitmapCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (BitmapCache*)*i;
		if( b->Filename == Filename )
		{
			b->Locks--;
			if( b->Locks <= 0 )
			{
				al_destroy_bitmap( b->Image );
				cache.erase( i );
				return;
			}
		}
	}
}

void BitmapCache::UnloadBitmap( ALLEGRO_BITMAP* Bitmap )
{
	BitmapCache* b;
	for( std::vector<BitmapCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (BitmapCache*)*i;
		if( b->Image == Bitmap )
		{
			b->Locks--;
			if( b->Locks <= 0 )
			{
				al_destroy_bitmap( b->Image );
				cache.erase( i );
				return;
			}
		}
	}
}
