#include "background.h"

namespace parallax
{
    void init(Background& background, sf::Texture& texture, float speedX, bool alignBottom, float offsetY)
    {
        texture.setRepeated(true);
        background.sprite.setTexture(texture);
        background.velocity = { speedX, 0.0f };
        background.currentX = 0.0f;

        sf::Vector2u texSize = texture.getSize();
        int stripHeight = texSize.y;

        if (texSize.y < externs::screenHeight)
        {
            stripHeight = texSize.y;
        }
        else
        {
            stripHeight = static_cast<int>(externs::screenHeight);
        }

        if (texSize.y < externs::screenHeight)
        {
            stripHeight = texSize.y;
        }
        else
        {
            stripHeight = static_cast<int>(externs::screenHeight);
        }

        background.sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<int>(externs::screenWidth), stripHeight)));

        float finalY = 0.0f;

        if (alignBottom)
        {
            finalY = externs::screenHeight - static_cast<float>(stripHeight);
        }
        else
        {
            finalY = 0.0f;
        }

        background.sprite.setPosition(sf::Vector2f(0.0f, finalY + offsetY));
    }

	void update(Background& background)
	{
		background.currentX += background.velocity.x * externs::deltaT;

		sf::IntRect currentRect = background.sprite.getTextureRect();
		currentRect.position.x = static_cast<int>(background.currentX);
		background.sprite.setTextureRect(currentRect);
	}

	void draw(sf::RenderWindow& window, Background background)
	{
		window.draw(background.sprite);
	}

	void setSpeed(Background& background, float x)
	{
		background.velocity.x = x;
	}
}