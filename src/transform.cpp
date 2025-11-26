#include "transform.h"

namespace trans
{
	void Translate(Transform& transform, vec::Vector2 delta)
	{
		transform.position += delta;
	}
}