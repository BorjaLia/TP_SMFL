#pragma once

#include "LLM_math.h"

namespace coll
{
	bool LineOnLine(vec::Vector2 line1Point1, vec::Vector2 line1Point2, vec::Vector2 line2Point1, vec::Vector2 line2Point2);
	bool PointOnRec(vec::Vector2 point, vec::Vector2 pos, vec::Vector2 size);
	bool PointOnCircle(vec::Vector2 point, vec::Vector2 pos, float radius);
	bool RecOnRec(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2);
	bool RecOnCircle(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
}