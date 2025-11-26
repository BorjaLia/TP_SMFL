#include "game.h"

#include <SFML/Graphics.hpp>

#include "globals.h"
#include "ground.h"
#include "car.h"
#include "collision.h"

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

	static void carCollision();
	static void wheelCollision();

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
		objects::car.wheels[0].isGrounded = false;

		delta::updateDeltaT();
		wheelCollision();
		carCollision();
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

	static void carCollision()
	{
		if (objects::car.transform.position.y > 500 - objects::car.collision.size.y)
		{
			objects::car.rigidBody.velocity.rotateDegree(180);
			objects::car.rigidBody.velocity *= 0.25f;
			std::cout << "out!\n";
		}
		if (objects::car.transform.position.y > 500 - (objects::car.collision.size.y * 2))
		{
			std::cout << "force!\n";
			rigidbody::AddForce(objects::car.rigidBody, { 0.0f,-globals::gravity * objects::car.rigidBody.mass * 5.0f });
		}
	}

	static void wheelCollision()
	{
		for (int i = 0; i < objects::ground.leftPart.shape.pointAmount - 1; i++)
		{
			vec::Vector2 dir = objects::ground.leftPart.shape.points[i + 1] - objects::ground.leftPart.shape.points[i];
			dir.normalize();

			vec::Vector2 norm = { -dir.y,dir.x };

			vec::Vector2 closestPoint;

			closestPoint.x = objects::car.wheels[0].offset.x + 10 + norm.x * 10; //cambiar x radio real
			closestPoint.y = objects::car.wheels[0].offset.y + 10 + norm.y * 10;

			if (coll::LineOnLine({ objects::car.wheels[0].offset.x, objects::car.wheels[0].offset.y}, closestPoint, objects::ground.leftPart.shape.points[i], objects::ground.leftPart.shape.points[i + 1]))
			{
				objects::car.wheels[0].isGrounded = true;
			}
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
