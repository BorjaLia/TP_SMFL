#include "game.h"

#include <SFML/Graphics.hpp>

#include "collision.h"
#include "globals.h"
#include "ground.h"
#include "car.h"
#include "label.h"

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
		label::Label verText;
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
	static void updateCamera();

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
		std::string asd = "res/font/ARIAL.TTF";
		if (!externs::fonts[0].openFromFile(asd.c_str()))
		{
			std::cout << "error!!! mirame!!";
		}
		
		objects::verText = label::init(vec::Vector2{ 1000, 300 }, "ver 0.1", label::Fonts::Default, 100, sf::Color::White);
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			{
				car::reset(objects::car, { externs::screenWidth,externs::screenHeight / 3.0f });
			}

			updateCamera();
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
			label::draw(objects::verText, objects::window);
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
			//objects::car.rigidBody.velocity.rotateDegree(180);
			//objects::car.rigidBody.velocity = 0.25f;
			std::cout << "out!\n";
		}
		if (objects::car.transform.position.y > 500 - (objects::car.collision.size.y * 2))
		{
			std::cout << "force!\n";
			//rigidbody::AddForce(objects::car.rigidBody, { 0.0f,-globals::gravity * objects::car.rigidBody.mass * 5.0f });
		}
		wheelCollision();
	}

	static void wheelCollision()
	{
		for (int i = 0; i < objects::car.wheels.size(); i++)
		{
			objects::car.wheels[i].isGroundedTimer -= externs::deltaT;
			if (objects::car.wheels[i].isGroundedTimer <= 0.0f)
			{
				objects::car.wheels[i].isGrounded = false;
			}
		}

		for (int i = 0; i < objects::ground.parts[0].shape.pointAmount - 1; i++)
		{
			vec::Vector2 p1 = objects::ground.parts[0].shape.points[i];
			vec::Vector2 p2 = objects::ground.parts[0].shape.points[i + 1];

			for (int j = 0; j < objects::car.wheels.size(); j++)
			{
				vec::Vector2 wheelPos = objects::car.wheels[j].transform.position + objects::car.wheels[j].offset;

				vec::Vector2 lineDir = p2 - p1;
				float lineLen = lineDir.magnitude();
				lineDir.normalize();

				vec::Vector2 toWheel = wheelPos - p1;
				float t = toWheel * lineDir;
				t = std::max(0.0f, std::min(lineLen, t));

				vec::Vector2 closestPoint = p1 + (lineDir * t);

				vec::Vector2 distVec = wheelPos - closestPoint;
				float dist = distVec.magnitude();

				if (dist < objects::car.wheels[j].collision.radius)
				{
					objects::car.wheels[j].isGroundedTimer = objects::car.wheels[j].isGroundedTimerLimit;
					objects::car.wheels[j].isGrounded = true;

					float penetration = objects::car.wheels[j].collision.radius - dist;
					vec::Vector2 normal = distVec;
					normal.normalize();

					objects::car.wheels[j].transform.position = objects::car.wheels[j].transform.position + (normal * penetration);

					float velAlongNormal = objects::car.wheels[j].rigidBody.velocity * normal;
					if (velAlongNormal < 0)
					{
						vec::Vector2 remove = normal * velAlongNormal;
						objects::car.wheels[j].rigidBody.velocity = objects::car.wheels[j].rigidBody.velocity - remove;

						objects::car.wheels[j].rigidBody.velocity.x *= 0.9f;
					}
				}
			}
		}
	}

	static void updateCamera()
	{
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		//{
		//	objects::camera.move({ 0.0f,-100.0f * externs::deltaT });
		//}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			objects::camera.move({ -100.0f * externs::deltaT ,0.0f });
		}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		//{
		//	objects::camera.move({ 0.0f,100.0f * externs::deltaT });
		//}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			objects::camera.move({ 100.0f * externs::deltaT ,0.0f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			objects::camera.zoom(1.0f + (externs::deltaT * 0.5f));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			objects::camera.zoom(1.0f - (externs::deltaT * 0.5f));
		}

		objects::camera.move({ ((1.25f * objects::car.transform.position.x) - objects::camera.getCenter().x) * 5.0f * externs::deltaT ,(objects::car.transform.position.y - objects::camera.getCenter().y) * 5.0f * externs::deltaT });

		objects::window.setView(objects::camera);
	}

	namespace delta
	{
		static void updateDeltaT()
		{
			externs::deltaT = delta::clock.restart().asSeconds();
		}
	}
}
