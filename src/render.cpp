#include "render.h"

#include <iostream>

#include "color.h"

namespace render
{
	static parallax::Background clouds;
	static parallax::Background hills;

	void init(car::Car& car)
	{
		if (!externs::cloudTexture.loadFromFile("res\\img\\background\\Clouds.png"))
		{
			std::cout << "Error loading Clouds.png" << std::endl;
		}

		if (!externs::hillTexture.loadFromFile("res\\img\\background\\SceneBG.png"))
		{
			std::cerr << "Error loading SceneBG.png" << std::endl;
		}

		if (!externs::carTexture.loadFromFile("res\\img\\car\\RedCar.png"))
		{
			std::cerr << "Error loading RedCar.png" << std::endl;
		}

		if (!externs::wheelTexture.loadFromFile("res\\img\\car\\Wheel.png"))
		{
			std::cerr << "Error loading Wheel.png" << std::endl;
		}

		if (!externs::driverSprite.loadFromFile("res\\img\\car\\LucasGalli.png"))
		{
			std::cerr << "Error loading LucasGalli.png :(" << std::endl;
		}

		car = car::init();

		parallax::init(clouds, externs::cloudTexture, 50.0f, false, -250.0f);
		parallax::init(hills, externs::hillTexture, 100.0f, true, 100.0f);
	}

	void update(car::Car& car)
	{
		parallax::update(clouds);
		parallax::update(hills);

		car::update(car);
	}

	void draw(sf::RenderWindow& window)
	{
		sf::View gameView = window.getView();

		window.setView(window.getDefaultView());

		parallax::draw(window, clouds);
		parallax::draw(window, hills);

		window.setView(gameView);
	}

	void drawThickLine(vec::Vector2 point1, vec::Vector2 point2, float outlineDrawWidth, sf::RenderWindow& window, color::ColorsName color)
	{
		vec::Vector2 startPoint = point1;
		vec::Vector2 endPoint = point2;
		vec::Vector2 line = endPoint - startPoint;

		sf::RectangleShape lineTemp = sf::RectangleShape({ line.magnitude(),outlineDrawWidth});

		sf::Vector2f tempVec = sf::Vector2f(point1.x, point1.y);

		lineTemp.setPosition(tempVec);
		lineTemp.setRotation(sf::radians(line.angle()));

		sf::Color drawColor = sf::Color({static_cast<uint8_t>(color::colors[static_cast<int>(color)].r),static_cast<uint8_t>(color::colors[static_cast<int>(color)].g),static_cast<uint8_t>(color::colors[static_cast<int>(color)].b),static_cast<uint8_t>(color::colors[static_cast<int>(color)].a) });
		lineTemp.setFillColor(drawColor);

		window.draw(lineTemp);
	}
}