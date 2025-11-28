#include "render.h"

#include <iostream>

namespace render
{
	static parallax::Background clouds;
	static parallax::Background hills;

	void init()
	{
		if (!externs::cloudTexture.loadFromFile("res\\img\\background\\Clouds.png"))
		{
			std::cout << "Error loading Clouds.png" << std::endl;

		}

		if (!externs::hillTexture.loadFromFile("res\\img\\background\\SceneBG.png"))
		{
			std::cerr << "Error loading SceneBG.png" << std::endl;

		}

		parallax::init(clouds, externs::cloudTexture, 50.0f, false, -250.0f);
		parallax::init(hills, externs::hillTexture, 100.0f, true, 100.0f);
	}

	void update()
	{
		parallax::update(clouds);
		parallax::update(hills);
	}

	void draw(sf::RenderWindow& window)
	{
		sf::View gameView = window.getView();

		window.setView(window.getDefaultView());

		parallax::draw(window, clouds);
		parallax::draw(window, hills);

		window.setView(gameView);
	}
}