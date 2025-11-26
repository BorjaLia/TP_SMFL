#pragma once

#include "SFML/Graphics.hpp"

#include "irregular.h"

namespace ground
{
	struct Ground
	{
		irregular::Irregular leftPart;
		irregular::Irregular middlePart;
		irregular::Irregular rightPart;
	};

	Ground init();
	void update(Ground& ground);
	void draw(Ground& ground, sf::RenderWindow& window);
}
