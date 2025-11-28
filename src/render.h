#pragma once

#include "background.h"
#include "car.h"

namespace render
{
	void init(car::Car& car);
	void update(car::Car& car);
	void draw(sf::RenderWindow& window);
}