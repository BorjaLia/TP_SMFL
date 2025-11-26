#pragma once

#include "LLM_math.h"
#include <vector>

namespace irregular
{
	struct Irregular
	{
		std::vector<vec::Vector2> points;
		int pointAmount = 0;
	};

	Irregular init(const vec::Vector2 points[], int pointAmount);;
}