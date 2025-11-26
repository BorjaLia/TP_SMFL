#include "game.h"

#include <SFML/Graphics.hpp>

#include "globals.h"
#include "ground.h"
#include "car.h"

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
		car::Car car;
	}

	static void game();
	static void init();
	static void deinit();
	static void update();
	static void draw();

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
			
			update();
			draw();
		}

		deinit();
	}

	static void init()
	{
		objects::window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned int>(externs::screenWidth), static_cast<unsigned int>(externs::screenHeight) }), "SFML works!");
		objects::ground = ground::init();
		objects::car = car::init();
	}

	static void deinit()
	{

	}

	static void update()
	{
		delta::updateDeltaT();

		ground::update(objects::ground);
		car::update(objects::car);
	}

	static void draw()
	{
		objects::window.clear();

		//ground::draw(objects::ground, objects::window);
		car::draw(objects::car, objects::window);

		objects::window.display();
	}

	namespace delta
	{
		static void updateDeltaT()
		{
			externs::deltaT = delta::clock.restart().asSeconds();
		}
	}
}
