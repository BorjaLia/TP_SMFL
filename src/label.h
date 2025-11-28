#pragma once

#include <SFML/Graphics.hpp>

#include "LLM_math.h"
#include "color.h"

namespace label
{
	struct Label
	{
		vec::Vector2 pos;
		std::string text;
		sf::Color color;
		sf::Font font;
		int fontSize = 0;
	};

	Label init(vec::Vector2 pos, std::string text, sf::Font& font, int fontSize, color::RGBColor color);
	Label init(vec::Vector2 pos, char text, sf::Font& font, int fontSize, color::RGBColor color);
	void draw(Label label, sf::RenderWindow& window);
}

