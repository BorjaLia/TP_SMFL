#include "globals.h"

namespace externs
{
	float deltaT = 0.0f;

	float screenWidth = 1500;
	float screenHeight = 820;

	sf::Font fonts[globals::maxFonts];

	sf::Texture placeholder = sf::Texture();

	sf::Texture cloudTexture;
	sf::Texture hillTexture;

	sf::Texture carTexture;
	sf::Texture wheelTexture;

	sf::Texture driverSprite;

	sf::Texture groundTexture;

	sf::SoundBuffer engineBuffer;
	sf::Sound engineSound{ engineBuffer };

	sf::SoundBuffer trickBuffer;
	sf::Sound trickSound{ trickBuffer };

	sf::SoundBuffer deathBuffer;
	sf::Sound deathSound{ deathBuffer };

	sf::SoundBuffer clapBuffer;
	sf::Sound clapSound{ clapBuffer };

	sf::Music backgroundMusic;

	sf::Font font;
}