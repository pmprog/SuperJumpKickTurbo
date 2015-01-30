
#pragma once

#include "ttffont.h"

class FontCache
{

	private:
		static std::vector<FontCache*> cache;
		
		FontCache( std::string File, int Size );

	public:
		std::string Filename;
		int FontSize;
		TTFFont* Font;
		int Locks;
		
		static TTFFont* LoadFont( std::string Filename, int Size );
		static void UnloadFont( TTFFont* Font );

};