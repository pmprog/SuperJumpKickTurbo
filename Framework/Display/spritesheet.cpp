
#include "spritesheet.h"

SpriteSheet::SpriteSheet( std::string Filename )
{
	sheet = BitmapCache::LoadBitmap( Filename );
}

SpriteSheet::SpriteSheet( std::string Filename, int FrameWidth, int FrameHeight )
{
	sheet = BitmapCache::LoadBitmap( Filename );

	for( int y = 0; y < al_get_bitmap_height( sheet ); y += FrameHeight )
	{
		for( int x = 0; x < al_get_bitmap_width( sheet ); x += FrameWidth )
		{
			AddSprite( x, y, FrameWidth, FrameHeight );
		}
	}
}

SpriteSheet::~SpriteSheet()
{
	BitmapCache::UnloadBitmap( sheet );
}

ALLEGRO_BITMAP* SpriteSheet::GetSheet()
{
	return sheet;
}

int SpriteSheet::AddSprite( int FrameX, int FrameY, int FrameWidth, int FrameHeight )
{
	SpriteSheetRegion* r = (SpriteSheetRegion*)malloc( sizeof(SpriteSheetRegion) );
	r->X = FrameX;
	r->Y = FrameY;
	r->Width = FrameWidth;
	r->Height = FrameHeight;
	frames.push_back( r );
	return frames.size() - 1;
}

void SpriteSheet::DrawSprite( int FrameNumber, int ScreenX, int ScreenY )
{
	if( FrameNumber < 0 || FrameNumber >= (int)frames.size() )
	{
		return;
	}

	SpriteSheetRegion* r = frames.at( FrameNumber );
	al_draw_bitmap_region( sheet, r->X, r->Y, r->Width, r->Height, ScreenX, ScreenY, 0 );
}

void SpriteSheet::DrawSprite( int FrameNumber, int ScreenX, int ScreenY, float ScaleX, float ScaleY, Angle* Rotation )
{
	if( FrameNumber < 0 || FrameNumber >= (int)frames.size() )
	{
		return;
	}

	SpriteSheetRegion* r = frames.at( FrameNumber );
	if( (Rotation == 0 || Rotation->ToDegrees() == 0) && ScaleX == 1.0f && ScaleY == 1.0f )
	{
		al_draw_bitmap_region( sheet, r->X, r->Y, r->Width, r->Height, ScreenX, ScreenY, 0 );
	} else {
		if( Rotation == 0 )
		{
			al_draw_tinted_scaled_rotated_bitmap_region( sheet, r->X, r->Y, r->Width, r->Height, al_map_rgba(255, 255, 255, 255), 0, 0, ScreenX, ScreenY, ScaleX, ScaleY, 0, 0 );
		} else {
			al_draw_tinted_scaled_rotated_bitmap_region( sheet, r->X, r->Y, r->Width, r->Height, al_map_rgba(255, 255, 255, 255), r->Width / 2, r->Height / 2, ScreenX + ((r->Width * ScaleX) / 2), ScreenY + ((r->Height * ScaleY) / 2), ScaleX, ScaleY, Rotation->ToRadians(), 0 );
		}
	}
}

void SpriteSheet::DrawSpritePortion( int FrameNumber, int ScreenX, int ScreenY, int FrameX, int FrameY, int Width, int Height )
{
	if( FrameNumber < 0 || FrameNumber >= (int)frames.size() )
	{
		return;
	}

	SpriteSheetRegion* r = frames.at( FrameNumber );
	al_draw_bitmap_region( sheet, r->X + FrameX, r->Y + FrameY, Width, Height, ScreenX, ScreenY, 0 );
}

SpriteSheetRegion* SpriteSheet::GetFrame( int FrameNumber )
{
	return frames.at( FrameNumber );
}

int SpriteSheet::GetFrameCount()
{
	return frames.size();
}
