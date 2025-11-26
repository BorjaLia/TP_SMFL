#pragma once

#include "LLM_math.h"

namespace trans
{
	struct  Transform
	{
		vec::Vector2 position = vec::Vector2(0.0f, 0.0f);
		vec::Vector2 scale = vec::Vector2(0.0f, 0.0f);

		float rotation = 0;
	};

	void Translate(Transform& transform, vec::Vector2 delta);
}