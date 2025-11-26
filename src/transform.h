#pragma once

#include "LLM_math.h"
#include "SFML/Graphics.hpp"

struct Transform
{
	vec::Vector2 translation = vec::Vector2(0.0f,0.0f);
	vec::Vector2 scale = vec::Vector2(0.0f, 0.0f);

	float rotation = 0.0f;
};