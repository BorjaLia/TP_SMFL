#include "game.h"

#include <SFML/Graphics.hpp>

#include "globals.h"
#include "ground.h"

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
		ground::Ground ground;
	}

	static void game();
	static void init();
	static void update();
	static void deinit();

	namespace delta
	{
		sf::Clock clock;
		static void updateDeltaT();
	}
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

		update();

		while (objects::window.isOpen())
		{
			while (const std::optional event = objects::window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
					objects::window.close();
			}

			objects::window.clear();

			//objects::window.draw();
			ground::draw(objects::ground, objects::window);

			objects::window.display();
		}

		deinit();
	}

	static void init()
	{
		objects::window = sf::RenderWindow(sf::VideoMode({ 1500, 820 }), "SFML works!");
		objects::ground = ground::init();
	}

	static void update()
	{
		delta::updateDeltaT();
	}

	static void deinit()
	{

	}

	namespace delta
	{
		static void updateDeltaT()
		{
			externs::deltaT = delta::clock.restart().asSeconds();
		}
	}
}
