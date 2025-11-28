#pragma once

#include "LLM_math.h"
#include "transform.h"
#include "regular.h" 

namespace coll
{
	struct RectCorners
	{
		vec::Vector2 tl, tr, br, bl;
	};

	struct CollisionResult
	{
		bool isColliding = false;
		float penetration = 0.0f;
		vec::Vector2 normal = { 0.0f, 0.0f };
		vec::Vector2 surfacePoint = { 0.0f, 0.0f };
	};

	RectCorners GetWorldCorners(const shape::Rectangle& rect, trans::Transform& t);

	CollisionResult PointVsLineSegment(vec::Vector2 point, vec::Vector2 lineStart, vec::Vector2 lineEnd);

	bool LineOnLine(vec::Vector2 line1Point1, vec::Vector2 line1Point2, vec::Vector2 line2Point1, vec::Vector2 line2Point2);
	bool PointOnRec(vec::Vector2 point, vec::Vector2 pos, vec::Vector2 size);
	bool PointOnCircle(vec::Vector2 point, vec::Vector2 pos, float radius);
	bool RecOnRec(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2);
	bool RecOnCircle(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
}