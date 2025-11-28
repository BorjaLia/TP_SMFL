#include "game.h"

#include <SFML/Graphics.hpp>

#include "collision.h"
#include "globals.h"
#include "ground.h"
#include "car.h"
#include "label.h"
#include "render.h"

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

	static bool ResolveRectVsGround(car::Car& car, shape::Rectangle& rect, bool isDeathBox);
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
		objects::roboto = sf::Font("res/font/Jumps Winter.ttf");
		objects::window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned int>(externs::screenWidth), static_cast<unsigned int>(externs::screenHeight) }), "Gil climb");
		objects::camera = objects::window.getView();
		objects::ground = ground::init();
		render::init(objects::car);
		objects::verText = label::init(vec::Vector2{ 1000, 300 }, "Gil climb", objects::roboto, 100, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
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

			render::update(objects::car);
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
		objects::window.clear(sf::Color(135, 206, 235));

		switch (scenes::currentScene)
		{
		case game::Scene::Playing:
		{
			render::draw(objects::window);
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

		for (int i = 0; i < objects::ground.parts[0].shape.pointAmount - 1; i++)
		{
			vec::Vector2 p1 = objects::ground.parts[0].shape.points[i];
			vec::Vector2 p2 = objects::ground.parts[0].shape.points[i + 1];

			for (int j = 0; j < objects::car.wheels.size(); j++)
			{
				vec::Vector2 wheelPos = objects::car.wheels[j].transform.position + objects::car.wheels[j].offset;
				float radius = objects::car.wheels[j].collision.radius;

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
						objects::car.wheels[j].isGroundedTimer = objects::car.wheels[j].isGroundedTimerLimit;
						objects::car.wheels[j].isGrounded = true;

						float penetration = radius - dist;
						vec::Vector2 normal = distVec;
						normal.normalize();

						objects::car.wheels[j].transform.position = objects::car.wheels[j].transform.position + (normal * penetration);

						float velAlongNormal = objects::car.wheels[j].rigidBody.velocity * normal;
						if (velAlongNormal < 0)
						{
							vec::Vector2 remove = normal * velAlongNormal;
							objects::car.wheels[j].rigidBody.velocity = objects::car.wheels[j].rigidBody.velocity - remove;

							// Friction
							//objects::car.wheels[j].rigidBody.velocity.x *= 0.99f;
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
		}
	}
}
