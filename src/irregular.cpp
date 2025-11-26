#include "irregular.h"

namespace irregular
{
	Irregular init(const vec::Vector2 points[], int pointAmount)
	{
		Irregular shape;

		for (int i = 0; i < pointAmount; i++)
		{
			shape.points.push_back(points[i]);
		}

		shape.pointAmount = pointAmount;

		return shape;
	}
}