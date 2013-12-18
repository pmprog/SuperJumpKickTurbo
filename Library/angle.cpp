
#include "angle.h"
#include "vector2.h"
#include <cmath>
#include "../Framework/graphicslib.h"

Angle::Angle()
{
	curAngle = 0;
}

Angle::Angle( float Degrees )
{
	curAngle = 0;
	Add( Degrees );
}

void Angle::Add( float Degrees )
{
	curAngle += Degrees;
	while( curAngle >= 360.0f )
	{
		curAngle -= 360.0f;
	}
	while( curAngle < 0.0f )
	{
		curAngle += 360.0f;
	}
}

float Angle::ToDegrees()
{
	return curAngle;
}

float Angle::ToRadians()
{
	return curAngle * (180.0f / M_PI);
}

float Angle::ToRadiansSparrow()
{
	return (curAngle / 360.0f) * (2 * SP_PI);
}

bool Angle::ClockwiseShortestTo( Angle* DestinationAngle )
{
	return ClockwiseShortestTo( DestinationAngle->ToDegrees() );
}

bool Angle::ClockwiseShortestTo( float DestinationAngle )
{
	float diff = DestinationAngle - curAngle;
	while( diff >= 360.0f )
	{
		diff -= 360.0f;
	}
	while( diff < 0.0f )
	{
		diff += 360.0f;
	}
	return (diff < 180.0f && diff > 0.0f);
}

void Angle::RotateShortestBy( Angle* DestinationAngle, float ByDegrees )
{
	return RotateShortestBy( DestinationAngle->ToDegrees(), ByDegrees );
}


void Angle::RotateShortestBy( float DestinationAngle, float ByDegrees )
{
	if( ClockwiseShortestTo( DestinationAngle ) )
	{
		Add( ByDegrees );
	} else {
		Add( -ByDegrees );
	}
}

float Angle::ShortestAngleTo( Angle* DestinationAngle )
{
	return ShortestAngleTo( DestinationAngle->ToDegrees() );
}

float Angle::ShortestAngleTo( float DestinationAngle )
{
	float ang = Maths::Min(Maths::Abs(DestinationAngle - curAngle), Maths::Abs(curAngle - DestinationAngle));
	return ang;
}

float Angle::Sin()
{
	return sin(ToRadians());
}

float Angle::Cos()
{
	return cos(ToRadians());
}

float Angle::Tan()
{
	return tan(ToRadians());
}

