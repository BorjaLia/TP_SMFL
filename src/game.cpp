#include "game.h"

#include <SFML/Graphics.hpp>

#include "collision.h"
#include "globals.h"
#include "ground.h"
#include "car.h"

namespace game
{
	enum class Scene
	{
		Playing,
		MainMenu,
		Credits,
		Rules,
		Exit
	};

	namespace objects
	{
		sf::View camera;
		sf::RenderWindow window;

		ground::Ground ground;
		car::Car car;
	}

	namespace scenes
	{
		static Scene currentScene = Scene::Playing;
		static Scene nextScene = Scene::Playing;
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
		objects::camera = objects::window.getView();
		objects::ground = ground::init();
		objects::car = car::init();
	}

	static void deinit()
	{

	}

	static void update()
	{
		delta::updateDeltaT();

		scenes::currentScene = scenes::nextScene;

		switch (scenes::currentScene)
		{
		case game::Scene::Playing:
		{
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			//{
			//	objects::camera.move({ 0.0f,-100.0f * externs::deltaT });
			//}
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			//{
			//	objects::camera.move({ -100.0f * externs::deltaT ,0.0f });
			//}
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			//{
			//	objects::camera.move({ 0.0f,100.0f * externs::deltaT });
			//}
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			//{
			//	objects::camera.move({ 100.0f * externs::deltaT ,0.0f });
			//}
			//
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			{
				objects::camera.zoom(1.0f + (externs::deltaT * 0.5f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			{
				objects::camera.zoom(1.0f - (externs::deltaT * 0.5f));
			}

			objects::camera.move({ (objects::car.transform.position.x - objects::camera.getCenter().x) * 5.0f * externs::deltaT ,(objects::car.transform.position.y - objects::camera.getCenter().y) * 2.5f * externs::deltaT });


			objects::window.setView(objects::camera);
			carCollision();
			ground::update(objects::ground, objects::car);
			car::update(objects::car);
			break;
		}
		case game::Scene::MainMenu:
		{

			break;
		}
		case game::Scene::Credits:
		{

			break;
		}
		case game::Scene::Rules:
		{

			break;
		}
		case game::Scene::Exit:
		{

			break;
		}
		default:
			break;
		}
	}

	static void draw()
	{
		objects::window.clear();

		switch (scenes::currentScene)
		{
		case game::Scene::Playing:
		{
			ground::draw(objects::ground, objects::window);
			car::draw(objects::car, objects::window);
			break;
		}
		case game::Scene::MainMenu:
		{

			break;
		}
		case game::Scene::Credits:
		{

			break;
		}
		case game::Scene::Rules:
		{

			break;
		}
		case game::Scene::Exit:
		{

			break;
		}
		default:
			break;
		}

		objects::window.display();
	}

	static void carCollision()
	{
		if (objects::car.transform.position.y > 500 - objects::car.collision.size.y)
		{
			objects::car.rigidBody.velocity.rotateDegree(180);
			objects::car.rigidBody.velocity = 0.25f;
			std::cout << "out!\n";
		}
		if (objects::car.transform.position.y > 500 - (objects::car.collision.size.y * 2))
		{
			std::cout << "force!\n";
			rigidbody::AddForce(objects::car.rigidBody, { 0.0f,-globals::gravity * objects::car.rigidBody.mass * 5.0f });
		}
		wheelCollision();
	}

	static void wheelCollision()
	{
		objects::car.wheels[0].isGrounded = false;

		for (int i = 0; i < objects::ground.parts[0].shape.pointAmount - 1; i++)
		{
			vec::Vector2 dir = objects::ground.parts[0].shape.points[i + 1] - objects::ground.parts[0].shape.points[i];
			dir.normalize();

			vec::Vector2 norm = { -dir.y,dir.x };

			vec::Vector2 closestPoint;

			closestPoint.x = objects::car.wheels[0].offset.x + 10 + norm.x * 10;
			closestPoint.y = objects::car.wheels[0].offset.y + 10 + norm.y * 10;

			if (coll::LineOnLine({ objects::car.wheels[0].offset.x, objects::car.wheels[0].offset.y }, closestPoint, objects::ground.parts[0].shape.points[i], objects::ground.parts[0].shape.points[i + 1]))
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
