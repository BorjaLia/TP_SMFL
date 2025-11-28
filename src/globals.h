#pragma once

#include <SFML/Graphics.hpp>

namespace globals
{
	const float gravity = 98.1f;
	static const int maxFonts = 1;
}

namespace externs
{
	extern float deltaT;

	extern float screenWidth;
	extern float screenHeight;

	extern sf::Font fonts[globals::maxFonts];
	extern sf::Texture placeholder;

	extern sf::Texture cloudTexture;
	extern sf::Texture hillTexture;

	extern sf::Texture carTexture;
	extern sf::Texture wheelTexture;

	extern sf::Texture driverSprite;
}