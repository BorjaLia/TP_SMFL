#include "label.h"

#include "globals.h"

namespace label
{
	Label init(vec::Vector2 pos, std::string text, sf::Font& font, int fontSize, color::RGBColor color)
	{
		Label label;

		label.pos = pos;
		label.text = text;
		label.font = font;
		label.fontSize = fontSize;
		label.color = sf::Color(static_cast<uint8_t>(color.r), static_cast<uint8_t>(color.g), static_cast<uint8_t>(color.b), static_cast<uint8_t>(color.a));

		return label;
	}

	Label init(vec::Vector2 pos, char text, sf::Font& font, int fontSize, color::RGBColor color)
	{
		Label label;

		label.pos = pos;
		label.text = text;
		label.font = font;
		label.fontSize = fontSize;
		label.color = sf::Color(static_cast<uint8_t>(color.r), static_cast<uint8_t>(color.g), static_cast<uint8_t>(color.b), static_cast<uint8_t>(color.a));

		return label;
	}

	void draw(Label label, sf::RenderWindow& window)
	{
		sf::Text text = sf::Text(label.font);
		
		text.setString(label.text.c_str());

		sf::Vector2 textPos = sf::Vector2(0.f,0.f);

		textPos.x = label.pos.x;
		textPos.y = label.pos.y;

		text.setPosition(textPos);
		text.setCharacterSize(label.fontSize);
		text.setFillColor(sf::Color(label.color));
		text.setStyle(sf::Text::Bold);

		window.draw(text);
	}
}