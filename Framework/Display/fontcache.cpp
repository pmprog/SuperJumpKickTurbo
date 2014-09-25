
#include "fontcache.h"

std::vector<FontCache*> FontCache::cache;

FontCache::FontCache( std::string File, int Size )
{
	Filename = File;
	Font = new TTFFont( File, Size );
	FontSize = Size;
	Locks = 0;
}

TTFFont* FontCache::LoadFont( std::string Filename, int Size )
{
	FontCache* b;
	for( std::vector<FontCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (FontCache*)*i;
		if( b->Filename == Filename && b->FontSize == Size )
		{
			b->Locks++;
			return b->Font;
		}
	}

	b = new FontCache( Filename, Size );
	cache.push_back( b );
	return b->Font;
}

void FontCache::UnloadFont( TTFFont* Font )
{
	FontCache* b;
	for( std::vector<FontCache*>::iterator i = cache.begin(); i != cache.end(); i++ )
	{
		b = (FontCache*)*i;
		if( b->Font == Font )
		{
			b->Locks--;
			if( b->Locks <= 0 )
			{
				delete b;
				cache.erase( i );
				return;
			}
		}
	}
}
