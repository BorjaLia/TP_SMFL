#pragma once

#include "LLM_math.h"
#include "globals.h"

namespace parallax
{
	struct Background
	{
		vec::Vector2 velocity;
		sf::Sprite sprite{ externs::placeholder };

		float currentX;
	};

	void init(Background& background, sf::Texture& texture, float speedX, bool alignBottom, float offsetY);
	void update(Background& background);
	void draw(sf::RenderWindow& window, Background background);

	void setSpeed(Background& background, float x);
}