#pragma once

#include <SFML/Graphics.hpp>

#include "LLM_math.h"

namespace label
{
	enum class Fonts
	{
		Default
	};

	struct Label
	{
		vec::Vector2 pos;
		std::string text;
		Fonts font = Fonts::Default;
		sf::Color color;
		int fontSize = 0;
	};

	Label init(vec::Vector2 pos, std::string text, Fonts font, int fontSize, sf::Color color);
	Label init(vec::Vector2 pos, char text, Fonts font, int fontSize, sf::Color color);
	void draw(Label label, sf::RenderWindow& window);
}

