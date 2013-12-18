
#pragma once

#include "maths.h"

class Angle
{
	private:
		float curAngle;

	public:
		Angle();
		Angle( float Degrees );
		void Add( float Degrees );
		float ToDegrees();
		float ToRadians();
		float ToRadiansSparrow();

		float ShortestAngleTo( Angle* DestinationAngle );
		float ShortestAngleTo( float DestinationAngle );
		bool ClockwiseShortestTo( Angle* DestinationAngle );
		bool ClockwiseShortestTo( float DestinationAngle );
		void RotateShortestBy( Angle* DestinationAngle, float ByDegrees );
		void RotateShortestBy( float DestinationAngle, float ByDegrees );

		float Sin();
		float Cos();
		float Tan();
};