#pragma once

#include "SFML/Graphics.hpp"

#include "irregular.h"
#include "car.h"

namespace ground
{
	const int totalParts = 3;

	struct Part
	{
		irregular::Irregular shape;
		bool canMove = false;
	};

	struct Ground
	{
		Part leftPart;
		Part middlePart;
		Part rightPart;

		bool isLooping = true;
	};

	Ground init();
	void update(Ground& ground, car::Car car);
	void draw(Ground& ground, sf::RenderWindow& window);
}
