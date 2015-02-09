
#include "vector2i.h"

Vector2i::Vector2i()
{
	X = 0;
	Y = 0;
}

Vector2i::Vector2i( int X, int Y )
{
	this->X = X;
	this->Y = Y;
}

Vector2i::Vector2i( Vector2i* Copy )
{
	X = Copy->X;
	Y = Copy->Y;
}

Vector2i Vector2i::operator+=(Vector2i A)
{
	X += A.X;
	Y += A.Y;
	return *this;
}

bool Vector2i::operator==( Vector2i A )
{
	return (this->X == A.X && this->Y == A.Y);
}

bool Vector2i::operator!=( Vector2i A )
{
	return (this->X != A.X || this->Y != A.Y);
}

void Vector2i::Add(Vector2i* Point)
{
	X += Point->X;
	Y += Point->Y;
}

void Vector2i::Subtract(Vector2i* Point)
{
	X -= Point->X;
	Y -= Point->Y;
}
