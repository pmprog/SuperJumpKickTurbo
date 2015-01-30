
#pragma once

#include "../includes.h"

class BitmapCache
{

	private:
		static std::vector<BitmapCache*> cache;
		
		BitmapCache( std::string File );


	public:
		std::string Filename;
		ALLEGRO_BITMAP* Image;
		int Locks;

		
		static ALLEGRO_BITMAP* LoadBitmap( std::string Filename );
		static void UnloadBitmap( std::string Filename );
		static void UnloadBitmap( ALLEGRO_BITMAP* Bitmap );

};