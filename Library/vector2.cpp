
#include "vector2.h"

Vector2::Vector2( float X, float Y )
{
  this->X = X;
  this->Y = Y;
}

Vector2::Vector2( Vector2* CopyFrom )
{
  X = CopyFrom->X;
  Y = CopyFrom->Y;
}

Vector2::Vector2( float AngleDegrees )
{
	float rotSin = sin(AngleDegrees * (M_PI / 180.0f));
	float rotCos = cos(AngleDegrees * (M_PI / 180.0f));
	X = rotCos - rotSin;
	Y = rotCos + rotSin;
}

void Vector2::RotateVector( float AngleDegrees )
{
  Vector2* tmp = new Vector2( 0, 0 );
  RotateVector( AngleDegrees, tmp );
  delete tmp;
}

void Vector2::RotateVector( float AngleDegrees, Vector2* RotationOrigin )
{
	float rotSin = sin(AngleDegrees * (M_PI / 180.0f));
	float rotCos = cos(AngleDegrees * (M_PI / 180.0f));
	float tmpX;
	tmpX = ((X - RotationOrigin->X) * rotCos) - ((Y - RotationOrigin->Y) * rotSin);
	Y = ((Y - RotationOrigin->Y) * rotCos) + ((X - RotationOrigin->X) * rotSin);
	X = tmpX;
}

bool Vector2::operator==(Vector2 A)
{
  return (this->X == A.X && this->Y == A.Y);
}

bool Vector2::operator!=(Vector2 A)
{
  return (this->X != A.X || this->Y != A.Y);
}

void Vector2::Add(Vector2* Point)
{
  X += Point->X;
  Y += Point->Y;
}

void Vector2::Multiply(float Multiplier)
{
  X *= Multiplier;
  Y *= Multiplier;
}

float Vector2::AngleTo( Vector2* CheckPoint )
{
  float r = atan2( CheckPoint->Y - Y, CheckPoint->X - X ) * 180.0f / M_PI;
	while( r >= 360.0f )
	{
		r -= 360.0f;
	}
	while( r < 0.0f )
	{
		r += 360.0f;
	}
  return r;
}

float Vector2::DistanceTo( Vector2* CheckPoint )
{
	return sqrt( pow(CheckPoint->X - X, 2.0f) + pow(CheckPoint->Y - Y, 2.0f) );
}

float Vector2::DotProduct( Vector2* a )
{
	return (X * a->X) + (Y * a->Y);
}

void Vector2::Normalise()
{
	float mag = sqrt( (X*X) + (Y*Y) );
	X /= mag;
	Y /= mag;
}