#include "button.h"

#include "color.h"
#include "globals.h"

namespace button
{
	static color::RGBColor normalColor = color::colors[static_cast<int>(color::ColorsName::SlightDarkWhite)];
	static color::RGBColor hoverColor = color::colors[static_cast<int>(color::ColorsName::MediumWhite)];
	static color::RGBColor pressedColor = color::colors[static_cast<int>(color::ColorsName::DarkWhite)];

	sf::Color toSFMLColor(color::RGBColor color)
	{
		return sf::Color(static_cast<uint8_t>(color.r), static_cast<uint8_t>(color.g), static_cast<uint8_t>(color.b));
	}

	Button init(float x, float y, float width, float height, std::string& text)
	{
		Button button;
		button.position.x = x;
		button.position.y = y;
		button.width = width;
		button.height = height;
		button.text = text;
		button.state = State::Normal;
		button.clicked = false;

		return button;
	}

	bool isMouseOnButton(sf::RenderWindow& window, Button& button)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, window.getDefaultView());

		return (worldPos.x >= button.position.x &&
			worldPos.x <= button.position.x + button.width &&
			worldPos.y >= button.position.y &&
			worldPos.y <= button.position.y + button.height);
	}

	void update(sf::RenderWindow& window, Button& button)
	{
		button.clicked = false;

		if (isMouseOnButton(window, button))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				button.state = State::Pressed;
			}
			else
			{
				if (button.state == State::Pressed)
				{
					button.clicked = true;
				}
				button.state = State::Hover;
			}
		}
		else
		{
			button.state = State::Normal;
		}
	}

	void draw(sf::RenderWindow& window, Button button, sf::Font roboto)
	{
		sf::RectangleShape rect;
		rect.setPosition({ button.position.x, button.position.y });
		rect.setSize(sf::Vector2f(button.width, button.height));

		switch (button.state)
		{
		case State::Normal:
			rect.setFillColor(toSFMLColor(normalColor));
			break;
		case State::Hover:
			rect.setFillColor(toSFMLColor(hoverColor));
			break;
		case State::Pressed:
			rect.setFillColor(toSFMLColor(pressedColor));
			break;
		}

		window.draw(rect);

		sf::Text textObj = { roboto, "Default", 20 };

		textObj.setString(button.text);
		textObj.setCharacterSize(20);

		textObj.setFillColor(sf::Color::White);

		sf::FloatRect textRect = textObj.getLocalBounds();

		textObj.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.size.y + textRect.position.y / 2.0f });

		textObj.setPosition({ button.position.x + button.width / 2.0f,button.position.y + button.height / 2.0f });

		window.draw(textObj);
	}
}