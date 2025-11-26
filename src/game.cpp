#include "game.h"

#include <SFML/Graphics.hpp>

namespace game
{
	enum class Scene
	{
		Playing,
		MainMenu,
		Pause,
		Credits,
		Rules,
		Exit
	};

	namespace objects
	{
		sf::RenderWindow window;
	}

	static void game();
	static void init();
	static void deinit();
}

namespace game //definiciones
{
	void runGame()
	{
		game();
	}

	static void game() //declaraciones
	{
		init();

		while (objects::window.isOpen())
		{
			while (const std::optional event = objects::window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					objects::window.close();
			}

			objects::window.clear();
			//objects::window.draw();
			objects::window.display();
		}

		deinit();
	}

	static void init()
	{
		objects::window = sf::RenderWindow(sf::VideoMode({ 200, 200 }), "SFML works!");
	}

	static void deinit()
	{

	}
}
