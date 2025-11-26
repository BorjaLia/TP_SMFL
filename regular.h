#pragma once

#include "LLM_math.h"

struct Rectangle
{
	vec::Vector2 pos = { 0.0f,0.f };
	float width = 0.0f;
	float height = 0.0f;
};

struct Circle
{
	vec::Vector2 pos = { 0.0f,0.f };
	float radius = 0.0f;
};