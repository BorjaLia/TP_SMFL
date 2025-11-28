#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

	extern sf::Texture groundTexture;

	extern sf::SoundBuffer engineBuffer;
	extern sf::Sound engineSound;

	extern sf::SoundBuffer trickBuffer;
	extern sf::Sound trickSound;

	extern sf::SoundBuffer deathBuffer;
	extern sf::Sound deathSound;

	extern sf::SoundBuffer clapBuffer;
	extern sf::Sound clapSound;

	extern sf::Music backgroundMusic;

	extern sf::Font font;
}