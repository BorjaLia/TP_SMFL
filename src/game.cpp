#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

#include "collision.h"
#include "globals.h"
#include "ground.h"
#include "car.h"
#include "label.h"
#include "render.h"
#include "button.h"

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
		static float cameraOffset = (0.25f * externs::screenWidth);
		sf::RenderWindow window;

		ground::Ground ground;
		car::Car car = car::Car();
		label::Label verText;
		sf::Font roboto;

		button::Button play;
		button::Button credits;
		button::Button exit;
	}

	namespace scenes
	{
		static Scene currentScene = Scene::MainMenu;
		static Scene nextScene = Scene::MainMenu;
	}

	static void game();
	static void init();
	static void deinit();
	static void update();
	static void draw();

	static bool ResolveRectVsGround(car::Car& car, shape::Rectangle& rect, bool isDeathBox);
	static void carCollision();
	static void wheelCollision();
	static void updateCamera();

	namespace delta
	{
		sf::Clock clock;
		static void updateDeltaT();
	}

	namespace sound
	{
		static void init();
		static void update();
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
		std::string playText = "Play";
		std::string creditsText = "Credits";
		std::string exitText = "Exit";

		objects::play = button::init(externs::screenWidth / 2.0f - 50.0f, 500.0f, 100.0f, 50.0f, playText);
		objects::credits = button::init(externs::screenWidth / 2.0f - 50.0f, 600.0f, 100.0f, 50.0f, creditsText);
		objects::exit = button::init(externs::screenWidth / 2.0f - 50.0f, 700.0f, 100.0f, 50.0f, exitText);

		sound::init();
		objects::roboto = sf::Font("res/font/Jumps Winter.ttf");
		objects::window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned int>(externs::screenWidth), static_cast<unsigned int>(externs::screenHeight) }), "Gil climb");
		objects::camera = objects::window.getView();
		objects::ground = ground::init();
		render::init(objects::car);
		objects::verText = label::init(vec::Vector2{ externs::screenWidth / 3.0f, 10.0f }, "Gil climb", objects::roboto, 100, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
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

			sound::update();
			render::update(objects::car);
			updateCamera();
			carCollision();
			ground::update(objects::ground, objects::car);
			car::update(objects::car);

			break;
		}
		case game::Scene::MainMenu:
		{
			button::update(objects::window, objects::play);
			button::update(objects::window, objects::credits);
			button::update(objects::window, objects::exit);

			if (objects::play.clicked)
			{
				scenes::nextScene = Scene::Playing;
				sound::init();
			}

			if (objects::credits.clicked)
			{
				scenes::nextScene = Scene::Credits;
			}

			if (objects::exit.clicked)
			{
				objects::window.close();
			}

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
		objects::window.clear(sf::Color(135, 206, 235));

		switch (scenes::currentScene)
		{
		case game::Scene::Playing:
		{
			render::draw(objects::window);
			ground::draw(objects::ground, objects::window);
			car::draw(objects::car, objects::window);

			sf::View currentView = objects::window.getView();
			objects::window.setView(objects::window.getDefaultView());

			objects::window.setView(currentView);

			break;
		}
		case game::Scene::MainMenu:
		{
			render::draw(objects::window);
			label::draw(objects::verText, objects::window);
			button::draw(objects::window, objects::play, objects::roboto);
			button::draw(objects::window, objects::credits, objects::roboto);
			button::draw(objects::window, objects::exit, objects::roboto);

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

	static bool ResolveRectVsGround(car::Car& car, shape::Rectangle& rect, bool isDeathBox)
	{
		coll::RectCorners corners = coll::GetWorldCorners(rect, car.transform);
		vec::Vector2 points[4] = { corners.tl, corners.tr, corners.br, corners.bl };

		bool hitDetected = false;

		for (int i = 0; i < objects::ground.parts[0].shape.pointAmount - 1; i++)
		{
			vec::Vector2 p1 = objects::ground.parts[0].shape.points[i];
			vec::Vector2 p2 = objects::ground.parts[0].shape.points[i + 1];

			for (int k = 0; k < 4; k++)
			{
				coll::CollisionResult result = coll::PointVsLineSegment(points[k], p1, p2);

				if (result.isColliding)
				{
					if (isDeathBox)
					{
						return true;
					}

					hitDetected = true;

					vec::Vector2 pushOut = result.normal * result.penetration;
					car.transform.position += pushOut;

					points[k] += pushOut;

					vec::Vector2 r = points[k] - car.transform.position;

					vec::Vector2 pointVel = car.rigidBody.velocity;
					pointVel.x += -car.rigidBody.angularVelocity * r.y;
					pointVel.y += car.rigidBody.angularVelocity * r.x;

					float relVel = pointVel * result.normal;

					if (relVel < 0.0f)
					{
						float e = 0.2f;
						float j = -(1.0f + e) * relVel;

						vec::Vector2 impulse = result.normal * j;

						car.rigidBody.velocity += impulse * 0.5f;

						vec::Vector2 tangent = { -result.normal.y, result.normal.x };
						float friction = 0.5f;
						float velTangent = pointVel * tangent;

						car.rigidBody.velocity += tangent * (-velTangent * friction);

						float torqueImpulse = (r.x * impulse.y - r.y * impulse.x);

						car.rigidBody.angularVelocity += torqueImpulse * 0.001f;
					}
				}
			}
		}
		return hitDetected;
	}

	static void carCollision()
	{
		if (ResolveRectVsGround(objects::car, objects::car.deathCollision, true))
		{
			objects::car.isAlive = false;
		}

		ResolveRectVsGround(objects::car, objects::car.collision, false);

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

		for (int i = 0; i < ground::totalParts; i++)
		{
			for (int j = 0; j < objects::ground.parts[i].shape.pointAmount - 1; j++)
			{
				vec::Vector2 p1 = objects::ground.parts[i].shape.points[j];
				vec::Vector2 p2 = objects::ground.parts[i].shape.points[j + 1];

				for (int k = 0; k < objects::car.wheels.size(); k++)
				{
					vec::Vector2 wheelPos = objects::car.wheels[k].transform.position + objects::car.wheels[k].offset;
					float radius = objects::car.wheels[k].collision.radius;

					vec::Vector2 lineDir = p2 - p1;
					float lineLen = lineDir.magnitude();
					lineDir.normalize();

					vec::Vector2 toWheel = wheelPos - p1;
					float t = toWheel * lineDir;

					if (t >= -radius && t <= lineLen + radius)
					{
						float tClamped = std::max(0.0f, std::min(lineLen, t));
						vec::Vector2 closestPoint = p1 + (lineDir * tClamped);

						if (wheelPos.y > closestPoint.y + radius)
						{
							float diff = closestPoint.y - (wheelPos.y - radius);
							objects::car.transform.position.y += diff;

							if (objects::car.rigidBody.velocity.y > 0)
							{
								objects::car.rigidBody.velocity.y = 0.0f;
							}

							continue;
						}

						vec::Vector2 distVec = wheelPos - closestPoint;
						float dist = distVec.magnitude();

						if (dist < radius)
						{
							objects::car.wheels[k].isGroundedTimer = objects::car.wheels[k].isGroundedTimerLimit;
							objects::car.wheels[k].isGrounded = true;

							float penetration = radius - dist;
							vec::Vector2 normal = distVec;
							normal.normalize();

							objects::car.wheels[k].transform.position = objects::car.wheels[k].transform.position + (normal * penetration);

							float velAlongNormal = objects::car.wheels[k].rigidBody.velocity * normal;
							if (velAlongNormal < 0)
							{
								vec::Vector2 remove = normal * velAlongNormal;
								objects::car.wheels[k].rigidBody.velocity = objects::car.wheels[k].rigidBody.velocity - remove;

								// Friction
								//objects::car.wheels[k].rigidBody.velocity.x *= 0.99f;
							}
						}
					}
				}
			}
		}
	}

	static void updateCamera()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			objects::camera.move({ -100.0f * externs::deltaT ,0.0f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			objects::camera.move({ 100.0f * externs::deltaT ,0.0f });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			objects::cameraOffset -= externs::deltaT * 500.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			objects::cameraOffset += externs::deltaT * 500.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			objects::camera.zoom(1.0f + (externs::deltaT * 0.5f));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			objects::camera.zoom(1.0f - (externs::deltaT * 0.5f));
		}

		objects::camera.move({ ((objects::cameraOffset + objects::car.transform.position.x) - objects::camera.getCenter().x) * 5.0f * externs::deltaT ,(objects::car.transform.position.y - objects::camera.getCenter().y) * 5.0f * externs::deltaT });

		objects::window.setView(objects::camera);
	}

	namespace delta
	{
		static void updateDeltaT()
		{
			externs::deltaT = delta::clock.restart().asSeconds();

			mth::Clamp(externs::deltaT, 0.0f, 0.1f);
		}
	}

	namespace sound
	{
		static void init()
		{
			if (!externs::engineBuffer.loadFromFile("res/sound/EngineSound.ogg"))
			{
				std::cout << "Error: engine sound not found";
			}
			externs::engineSound.setBuffer(externs::engineBuffer);

			externs::engineSound.setLooping(true);
			externs::engineSound.setVolume(20.0f);

			if (scenes::nextScene == Scene::Playing)
			{
				externs::engineSound.play();
			}

			if (!externs::backgroundMusic.openFromFile("res/sound/music.ogg"))
			{
				std::cout << "Error: loading music";
			}

			externs::backgroundMusic.setLooping(true);
			externs::backgroundMusic.setVolume(5.0f);

			if (externs::backgroundMusic.getStatus() != sf::SoundSource::Status::Playing)
			{
				externs::backgroundMusic.play();
			}
		}

		static void update()
		{
			float speed = std::abs(objects::car.rigidBody.velocity.x);
			float pitch = 1.0f + (speed / 1000.0f);

			externs::engineSound.setPitch(pitch);
		}
	}
}
