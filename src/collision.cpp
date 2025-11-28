#include "collision.h"

namespace coll
{
	RectCorners GetWorldCorners(const shape::Rectangle& rect, trans::Transform& t)
	{
		RectCorners c;
		float halfW = rect.size.x / 2.0f;
		float halfH = rect.size.y / 2.0f;

		vec::Vector2 p_tl = { -halfW + rect.pos.x, -halfH + rect.pos.y };
		vec::Vector2 p_tr = { halfW + rect.pos.x, -halfH + rect.pos.y };
		vec::Vector2 p_br = { halfW + rect.pos.x, halfH + rect.pos.y };
		vec::Vector2 p_bl = { -halfW + rect.pos.x, halfH + rect.pos.y };

		c.tl = t.position + p_tl.rotated(t.rotation);
		c.tr = t.position + p_tr.rotated(t.rotation);
		c.br = t.position + p_br.rotated(t.rotation);
		c.bl = t.position + p_bl.rotated(t.rotation);

		return c;
	}

	CollisionResult PointVsLineSegment(vec::Vector2 point, vec::Vector2 lineStart, vec::Vector2 lineEnd)
	{
		CollisionResult result;

		vec::Vector2 lineVec = lineEnd - lineStart;
		vec::Vector2 lineDir = lineVec.normalized();
		float segmentLen = lineVec.magnitude();

		vec::Vector2 normal = { lineDir.y, -lineDir.x };

		vec::Vector2 toPoint = point - lineStart;

		float t = toPoint * lineDir;

		float buffer = 20.0f;

		if (t >= -buffer && t <= segmentLen + buffer)
		{
			float tClamped = std::max(0.0f, std::min(segmentLen, t));
			vec::Vector2 closestOnLine = lineStart + (lineDir * tClamped);

			vec::Vector2 diff = point - closestOnLine;

			float distSigned = diff * normal;

			if (distSigned < 0.0f)
			{
				result.isColliding = true;
				result.penetration = -distSigned;
				result.normal = normal;
				result.surfacePoint = closestOnLine;
			}
		}

		return result;
	}

	bool LineOnLine(vec::Vector2 line1Point1, vec::Vector2 line1Point2, vec::Vector2 line2Point1, vec::Vector2 line2Point2)
	{
		vec::Vector2 a = line1Point1;
		vec::Vector2 a2 = line1Point2;
		vec::Vector2 b = line2Point1;
		vec::Vector2 b2 = line2Point2;

		float uA = ((b2.x - b.x) * (a.y - b.y) - (b2.y - b.y) * (a.x - b.x)) /
			((b2.y - b.y) * (a2.x - a.x) - (b2.x - b.x) * (a2.y - a.y));

		float uB = ((a2.x - a.x) * (a.y - b.y) - (a2.y - a.y) * (a.x - b.x)) /
			((b2.y - b.y) * (a2.x - a.x) - (b2.x - b.x) * (a2.y - a.y));

		return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
	}

	bool PointOnRec(vec::Vector2 point, vec::Vector2 pos, vec::Vector2 size)
	{
		if (!(point.x > pos.x - size.x / 2.0f && point.x < pos.x + size.x / 2.0f))
		{
			return false;
		}
		if (!(point.y > pos.y - size.y / 2.0f && point.y < pos.y + size.y / 2.0f))
		{
			return false;
		}
		return true;
	}

	bool PointOnCircle(vec::Vector2 point, vec::Vector2 pos, float radius)
	{
		if ((point - pos).magnitude() < radius)
		{
			return true;
		}
		return false;
	}

	bool RecOnRec(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point)
	{
		vec::Vector2 half1 = { size1.x * 0.5f, size1.y * 0.5f };
		vec::Vector2 half2 = { size2.x * 0.5f, size2.y * 0.5f };

		float left1 = pos1.x - half1.x;
		float right1 = pos1.x + half1.x;
		float top1 = pos1.y - half1.y;
		float bottom1 = pos1.y + half1.y;

		float left2 = pos2.x - half2.x;
		float right2 = pos2.x + half2.x;
		float top2 = pos2.y - half2.y;
		float bottom2 = pos2.y + half2.y;

		float overlapLeft = mth::Max(left1, left2);
		float overlapRight = mth::Min(right1, right2);
		float overlapTop = mth::Max(top1, top2);
		float overlapBottom = mth::Min(bottom1, bottom2);

		if (overlapLeft >= overlapRight || overlapTop >= overlapBottom)
		{
			return false;
		}

		point.x = (overlapLeft + overlapRight) * 0.5f;
		point.y = (overlapTop + overlapBottom) * 0.5f;

		return true;
	}

	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2, vec::Vector2& point)
	{
		vec::Vector2 distance = { pos1.x - pos2.x, pos1.y - pos2.y };
		distance = { distance.x * distance.x, distance.y * distance.y };

		if ((distance.x + distance.y) <= ((radius1 + radius2) * (radius1 + radius2)))
		{
			point = (pos1 + pos2) * 0.5f;
			return true;
		}
		return false;
	}

	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2)
	{
		vec::Vector2 point;
		return CircleOnCircle(pos1, pos2, radius1, radius2, point);
	}

	bool RecOnCircle(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point)
	{
		vec::Vector2 closest = pos2;

		if (pos2.x < pos1.x - size1.x / 2.0f)
		{
			closest.x = pos1.x - size1.x / 2.0f;
		}
		if (pos2.x > pos1.x + size1.x / 2.0f)
		{
			closest.x = pos1.x + size1.x / 2.0f;
		}

		if (pos2.y < pos1.y - size1.y / 2.0f)
		{
			closest.y = pos1.y - size1.y / 2.0f;
		}
		if (pos2.y > pos1.y + size1.y / 2.0f)
		{
			closest.y = pos1.y + size1.y / 2.0f;
		}

		vec::Vector2 distance = pos2 - closest;

		float distanceSquared = (distance.x * distance.x) + (distance.y * distance.y);

		point = closest + distance * 0.5f;

		return distanceSquared <= (size2.x * size2.x);
	}
}