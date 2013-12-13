#pragma once

#include "vector2.h"
#include "list.h"

// Collision Polygon
class Polyhedron
{
	public:
		List* Points;
		List* Edges;
		Vector2* Centre;

		Polyhedron();
		void Compute();

		bool HitTest(Vector2* Point);
		bool DoesCollide(Vector2* Velocity, Polyhedron* CheckWith);
		Polyhedron* Translate( Vector2* Offset, int FlipX, float Rotation );
		Polyhedron* Translate( float OffsetX, float OffsetY, int FlipX, float Rotation );

		void Project( Vector2* axis, float* min, float* max );
		float IntervalDistance(float minA, float maxA, float minB, float maxB);
};
