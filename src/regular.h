#pragma once

#include "LLM_math.h"

namespace shape
{
	struct Rectangle
	{
		vec::Vector2 pos = { 0.0f,0.f };
		vec::Vector2 size = { 0.0f,0.f };
	};

	struct Circle
	{
		vec::Vector2 pos = { 0.0f,0.f };
		float radius = 0.0f;
	};
}