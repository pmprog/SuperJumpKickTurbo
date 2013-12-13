
#pragma once

#include "maths.h"

class Vector2
{
  public:
    float X;
    float Y;

    Vector2( float X, float Y );
    Vector2( Vector2* CopyFrom );
    Vector2( float AngleDegrees );
    void RotateVector( float AngleDegrees );
    void RotateVector( float AngleDegrees, Vector2* RotationOrigin );

    bool operator==(Vector2 A);
    bool operator!=(Vector2 A);

    void Add(Vector2* Point);
    void Multiply(float Multiplier);
    float AngleTo( Vector2* CheckPoint );
		float DistanceTo( Vector2* CheckPoint );
		float DotProduct( Vector2* a );
		void Normalise();


};

