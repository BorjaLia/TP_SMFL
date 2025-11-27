#include "label.h"

#include "globals.h"

namespace label
{
	Label init(vec::Vector2 pos, std::string text, sf::Font& font, int fontSize, sf::Color color)
	{
		Label label;

		label.pos = pos;
		label.text = text;
		label.font = font;
		label.fontSize = fontSize;
		label.color = sf::Color(color);

		return label;
	}

	Label init(vec::Vector2 pos, char text, sf::Font& font, int fontSize, sf::Color color)
	{
		Label label;

		label.pos = pos;
		label.text = text;
		label.font = font;
		label.fontSize = fontSize;
		label.color = sf::Color(color);

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