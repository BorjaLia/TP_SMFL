#pragma once

#include "background.h"
#include "car.h"
#include "color.h"

namespace render
{
	void init(car::Car& car);
	void update(car::Car& car);
	void draw(sf::RenderWindow& window);
	void drawThickLine(vec::Vector2 point1, vec::Vector2 point2, float outlineDrawWidth, sf::RenderWindow& window, color::ColorsName color);
}