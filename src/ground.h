#pragma once

#include "SFML/Graphics.hpp"

#include "irregular.h"
#include "car.h"

namespace ground
{
	const int totalParts = 2;

	struct Part
	{
		irregular::Irregular shape;
		bool canMove = false;
		vec::Vector2 limitLine[2];
	};

	enum class PartName
	{
		Left,
		Right
	};

	struct Ground
	{
		Part parts[totalParts];

		bool isLooping = true;
	};

	Ground init();
	void update(Ground& ground, car::Car car);
	void draw(Ground& ground, sf::RenderWindow& window);
}
