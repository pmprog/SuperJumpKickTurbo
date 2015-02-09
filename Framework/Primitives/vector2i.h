
#pragma once

#include "../maths.h"

class Vector2i
{
	public:
		int X;
		int Y;

		Vector2i();
		Vector2i( int X, int Y );
		Vector2i( Vector2i* Copy );

		Vector2i operator+=(Vector2i A);

		bool operator==(Vector2i A);
		bool operator!=(Vector2i A);

		void Add(Vector2i* Point);
		void Subtract(Vector2i* Point);
};

