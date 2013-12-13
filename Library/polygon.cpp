#include "polygon.h"
#include "maths.h"

Polyhedron::Polyhedron()
{
	Points = new List();
	Edges = new List();
	Centre = new Vector2( 0, 0 );
}

void Polyhedron::Compute()
{
	Centre->X = 0;
	Centre->Y = 0;
	if( Edges->count > 0 )
	{
		while( Edges->count > 0 )
		{
			delete Edges->ItemAt( 0 );
			Edges->RemoveFirst();
		}
	}

	for( int idx = 0; idx < Points->count; idx++ )
	{
		Vector2* p = Points->ItemAt<Vector2*>(idx);
		Vector2* p2 = Points->ItemAt<Vector2*>( (idx + 1) % Points->count );

		Centre->X += p->X; Centre->Y += p->Y;		
		Edges->AddToEnd( (void*)(new Vector2( p2->X - p->X, p2->Y - p->Y )) );
	}
	Centre->X /= Points->count;
	Centre->Y /= Points->count;
}

/**********************************************************
		C# code from 
		http://www.codeproject.com/Articles/15573/2D-Polygon-Collision-Detection
*/
bool Polyhedron::DoesCollide(Vector2* Velocity, Polyhedron* CheckWith)
{
	bool Intersect = true;
	bool WillIntersect = true;

	int edgeCountA = Edges->count;
	int edgeCountB = CheckWith->Edges->count;
	// float minIntervalDistance = 99999999;
	// Vector2* translationAxis = new Vector2( 0, 0 );
	Vector2* edge;

	// Loop through all the edges of both polygons
	for( int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++ )
	{
		if( edgeIndex < edgeCountA )
		{
			edge = Edges->ItemAt<Vector2*>(edgeIndex);
		} else {
			edge = CheckWith->Edges->ItemAt<Vector2*>(edgeIndex - edgeCountA);
		}

		// ===== 1. Find if the polygons are currently intersecting =====

		// Find the axis perpendicular to the current edge
		Vector2* axis = new Vector2(-edge->Y, edge->X);
		axis->Normalise();

		// Find the projection of the polygon on the current axis
		float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
		Project(axis, &minA, &maxA);
		CheckWith->Project(axis, &minB, &maxB);

		// Check if the polygon projections are currentlty intersecting
		if( IntervalDistance(minA, maxA, minB, maxB) > 0 )
		{
			Intersect = false;
		}

		// ===== 2. Now find if the polygons *will* intersect =====

		// Project the velocity on the current axis
		float velocityProjection = axis->DotProduct( Velocity );

		// Get the projection of polygon A during the movement
		if( velocityProjection < 0 )
		{
			minA += velocityProjection;
		} else {
			maxA += velocityProjection;
		}

		// Do the same test as above for the new projection
		float intervalDistance = IntervalDistance( minA, maxA, minB, maxB );
		if( intervalDistance > 0 )
		{
			WillIntersect = false;
		}

		delete axis;

		// If the polygons are not intersecting and won't intersect, exit the loop
		if( !Intersect && !WillIntersect)
		{
			break;
		}

		/*
		// Check if the current interval distance is the minimum one. If so store
		// the interval distance and the current distance.
		// This will be used to calculate the minimum translation vector
		intervalDistance = Math.Abs(intervalDistance);
		if (intervalDistance < minIntervalDistance) {
			minIntervalDistance = intervalDistance;
			translationAxis = axis;

			Vector d = polygonA.Center - polygonB.Center;
			if (d.DotProduct(translationAxis) < 0) translationAxis = -translationAxis;
		}
		*/
		int xcv = 1;
		
	}

	// The minimum translation vector can be used to push the polygons appart.
	// First moves the polygons by their velocity
	// then move polygonA by MinimumTranslationVector.
	// if (result.WillIntersect) result.MinimumTranslationVector = translationAxis * minIntervalDistance;

	return (Intersect | WillIntersect);
}

Polyhedron* Polyhedron::Translate( Vector2* Offset, int FlipX, float Rotation )
{
	return Translate( Offset->X, Offset->Y, FlipX, Rotation );
}

Polyhedron* Polyhedron::Translate( float OffsetX, float OffsetY, int FlipX, float Rotation )
{
	float cosR = cos(-Rotation * (M_PI / 180.0f));
	float sinR = sin(-Rotation * (M_PI / 180.0f));

	Polyhedron* newPoly = new Polyhedron;
	for( int idx = 0; idx < Points->count; idx++ )
	{
		Vector2* working = new Vector2( Points->ItemAt<Vector2*>(idx)->X * ( FlipX != 0 ? -1 : 1 ), Points->ItemAt<Vector2*>(idx)->Y );

		if( Rotation != 0.0f )
		{
			float vx = (cosR * working->X) - (sinR * working->Y);
			float vy = (sinR * working->X) + (cosR * working->Y);
			working->X = vx;
			working->Y = vy;
		}
		working->X += OffsetX;
		working->Y += OffsetY;

		newPoly->Points->AddToEnd( (void*)working );
	}
	newPoly->Compute();

	return newPoly;
}

/**********************************************************
		C# code from 
		http://www.codeproject.com/Articles/15573/2D-Polygon-Collision-Detection
*/
// Calculate the projection of a polygon on an axis and returns it as a [min, max] interval
void Polyhedron::Project( Vector2* axis, float* min, float* max )
{
	// To project a point on an axis use the dot product
	float d = axis->DotProduct(Points->ItemAt<Vector2*>(0));
	*min = d;
	*max = d;
	for (int i = 0; i < Points->count; i++)
	{
		d = Points->ItemAt<Vector2*>(i)->DotProduct(axis);
		if (d < *min)
		{
			*min = d;
		} else if (d > *max) {
			*max = d;
		}
	}
}

/**********************************************************
		C# code from 
		http://www.codeproject.com/Articles/15573/2D-Polygon-Collision-Detection
*/
// Calculate the distance between [minA, maxA] and [minB, maxB]
// The distance will be negative if the intervals overlap
float Polyhedron::IntervalDistance(float minA, float maxA, float minB, float maxB)
{
	if (minA < minB)
	{
		return minB - maxA;
	} else {
		return minA - maxB;
	}
}

bool Polyhedron::HitTest(Vector2* Point)
{
	int i;
	int j;
	bool odd = false;

	j = Points->count;

	for( i = 0; i < Points->count; i++ )
	{
		Vector2* pI = Points->ItemAt<Vector2*>(i);
		Vector2* pJ = Points->ItemAt<Vector2*>(j);

		if( (pI->Y < Point->Y && pJ->Y >= Point->Y) || (pJ->Y < Point->Y && pI->Y >= Point->Y) )
		{
			if( pI->X + (Point->Y - pI->Y) / (pJ->Y - pI->Y) * (pJ->X - pI->X) < Point->X )
			{
				odd = !odd;
			}
		}

		j = i;
	}

	return odd;
}
