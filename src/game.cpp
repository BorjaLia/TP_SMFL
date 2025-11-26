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

	static void collision();

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
		collision();
		ground::update(objects::ground);
		car::update(objects::car);
	}

	static void draw()
	{
		objects::window.clear();

		ground::draw(objects::ground, objects::window);
		car::draw(objects::car, objects::window);

		objects::window.display();
	}

	static void collision()
	{
		if (objects::car.transform.position.y > 500 - objects::car.collision.size.y)
		{
			objects::car.rigidBody.velocity.rotateDegree(180);
			objects::car.rigidBody.velocity *= 0.25f;
			std::cout << "out!\n";
		}
		if (objects::car.transform.position.y > 500 - (objects::car.collision.size.y*2))
		{
			std::cout << "force!\n";
			rigidbody::AddForce(objects::car.rigidBody, { 0.0f,-globals::gravity * objects::car.rigidBody.mass * 5.0f});
		}
	}

	namespace delta
	{
		static void updateDeltaT()
		{
			externs::deltaT = delta::clock.restart().asSeconds();
		}
	}
}
