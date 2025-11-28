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
		Death,
		Exit
	};

	enum class CreditsText
	{
		Credits,
		Programmers,
		Assets,
		Eluney,
		Borja,
		Laure,
		Movious,
		Lucy,
		Pou
	};

	enum class RulesText
	{
		Rules,
		Explanation1,
		Explanation2,
		Controls,
		LeftClick,
		MoveLeft,
		MoveRight,
		Pause
	};

	namespace objects
	{
		static const int maxCreditsText = 9;
		static const int maxRulesText = 8;

		sf::View camera;
		static float cameraOffset = (0.25f * externs::screenWidth);
		sf::RenderWindow window;

		label::Label creditsText[maxCreditsText];
		label::Label rulesText[maxRulesText];
		ground::Ground ground;
		car::Car car = car::Car();
		label::Label verText;

		sf::Font roboto;

		button::Button pause;

		label::Label deathTitle;
		label::Label deathScoreLabel;
		label::Label restartLabel;
		button::Button deathMenu;
		button::Button deathExit;

		button::Button play;
		button::Button credits;
		button::Button rules;
		button::Button exit;
		button::Button back;
	}

	namespace scenes
	{
		static Scene currentScene = Scene::MainMenu;
		static Scene nextScene = Scene::MainMenu;
	}

	static void game();
	static void init();
	static void update();
	static void draw();

	static bool ResolveRectVsGround(car::Car& car, shape::Rectangle& rect, bool isDeathBox);
	static void carCollision();
	static void wheelCollision();
	static void updateCamera();

	static bool wasPausedPressed = false;
	static bool isPausedPressed = false;
	static bool isPaused = false;

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
	}

	static void init()
	{
		std::string pauseText = "Pause";
		std::string menu = "Menu";
		std::string playText = "Play";
		std::string creditsText = "Credits";
		std::string rulesText = "Rules";
		std::string exitText = "Exit";
		std::string backText = "Back";
		std::string creditsText1 = "Credits";
		std::string creditsText2 = "Programmers";
		std::string creditsText3 = "Assets";
		std::string creditsText4 = "Eluney Jazmin Mousseigne";
		std::string creditsText5 = "Borja Lia Deichler";
		std::string creditsText6 = "Agustin Ezequiel Laure";
		std::string creditsText7 = "Movious (Lucas Galli)";
		std::string creditsText8 = "lucylavend - itch.io";
		std::string creditsText9 = "Pou - music";

		std::string rulesText1 = "Rules";
		std::string rulesText2 = "-Drive as far as you can and perform tricks to gain points but";
		std::string rulesText3 = " be CAREFUL, if your head touches the ground, you die";
		std::string rulesText4 = "Controls";
		std::string rulesText5 = "-Left Click: Select";
		std::string rulesText6 = "-A: Accelerate left";
		std::string rulesText7 = "-D: Accelerate right";
		std::string rulesText8 = "-ESC: Pause";

		std::string deathTitleStr = "YOU DIED";
		std::string restartStr = "Press 'R' to Restart";

		objects::play = button::init(externs::screenWidth / 2.0f - 50.0f, 400.0f, 100.0f, 50.0f, playText);
		objects::pause = button::init(externs::screenWidth / 2.0f - 50.0f, 500.0f, 100.0f, 50.0f, pauseText);
		objects::rules = button::init(externs::screenWidth / 2.0f - 50.f, 500, 100.f, 50.f, rulesText);
		objects::credits = button::init(externs::screenWidth / 2.0f - 50.0f, 600.0f, 100.0f, 50.0f, creditsText);
		objects::exit = button::init(externs::screenWidth / 2.0f - 50.0f, 700.0f, 100.0f, 50.0f, exitText);
		objects::back = button::init(externs::screenWidth / 16.f, 700, 100, 50.f, backText);

		sound::init();
		objects::roboto = sf::Font("res/font/Jumps Winter.ttf");
		objects::window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned int>(externs::screenWidth), static_cast<unsigned int>(externs::screenHeight) }), "Gil climb");
		objects::camera = objects::window.getView();
		objects::ground = ground::init();
		render::init(objects::car);
		objects::creditsText[static_cast<int>(CreditsText::Credits)] = label::init({ externs::screenWidth / 2.f - 100.f,100.f }, creditsText1, objects::roboto, 50, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::creditsText[static_cast<int>(CreditsText::Programmers)] = label::init({ externs::screenWidth / 5 - 37,275.f }, creditsText2, objects::roboto, 37, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::creditsText[static_cast<int>(CreditsText::Assets)] = label::init({ externs::screenWidth - externs::screenWidth / 3 + 12.f,275.f }, creditsText3, objects::roboto, 37, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::creditsText[static_cast<int>(CreditsText::Eluney)] = label::init({ externs::screenWidth / 5 - 17,420.f }, creditsText4, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::creditsText[static_cast<int>(CreditsText::Borja)] = label::init({ externs::screenWidth / 5 - 17,520.f }, creditsText5, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::creditsText[static_cast<int>(CreditsText::Laure)] = label::init({ externs::screenWidth / 5 - 17,620.f }, creditsText6, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::creditsText[static_cast<int>(CreditsText::Movious)] = label::init({ externs::screenWidth - externs::screenWidth / 3 + 12.f,400.f }, creditsText7, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::creditsText[static_cast<int>(CreditsText::Lucy)] = label::init({ externs::screenWidth - externs::screenWidth / 3 + 12.f,500.f }, creditsText8, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::creditsText[static_cast<int>(CreditsText::Pou)] = label::init({ externs::screenWidth - externs::screenWidth / 3 + 12.f,600.f }, creditsText9, objects::roboto, 17, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);

		objects::rulesText[static_cast<int>(RulesText::Rules)] = label::init({ externs::screenWidth / 2.f - 82.f, 100.f }, rulesText1, objects::roboto, 50, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::rulesText[static_cast<int>(RulesText::Explanation1)] = label::init({ 140, 225.f }, rulesText2, objects::roboto, 28, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::rulesText[static_cast<int>(RulesText::Explanation2)] = label::init({ 140, 300.f }, rulesText3, objects::roboto, 28, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::rulesText[static_cast<int>(RulesText::Controls)] = label::init({ externs::screenWidth / 2.f - 100.f, 400.f }, rulesText4, objects::roboto, 39, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::rulesText[static_cast<int>(RulesText::LeftClick)] = label::init({ externs::screenWidth / 2.f - 100.f, 500.f }, rulesText5, objects::roboto, 15, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::rulesText[static_cast<int>(RulesText::MoveLeft)] = label::init({ externs::screenWidth / 2.f - 100.f, 550.f }, rulesText6, objects::roboto, 15, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::rulesText[static_cast<int>(RulesText::MoveRight)] = label::init({ externs::screenWidth / 2.f - 100.f, 600.f }, rulesText7, objects::roboto, 15, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);
		objects::rulesText[static_cast<int>(RulesText::Pause)] = label::init({ externs::screenWidth / 2.f - 100.f, 650.f }, rulesText8, objects::roboto, 15, color::colors[static_cast<int>(color::ColorsName::DarkGreen)]);

		objects::deathMenu = button::init(externs::screenWidth / 2.0f - 50.0f, 600.0f, 100.0f, 50.0f, menu);
		objects::deathExit = button::init(externs::screenWidth / 2.0f - 50.0f, 700.0f, 100.0f, 50.0f, exitText);

		objects::deathTitle = label::init({ externs::screenWidth / 2.f - 200.f, 150.f }, deathTitleStr, objects::roboto, 80, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
		objects::restartLabel = label::init({ externs::screenWidth / 2.f - 200.f, 450.f }, restartStr, objects::roboto, 30, color::colors[static_cast<int>(color::ColorsName::DarkWhite)]);
		objects::deathScoreLabel = label::init({ externs::screenWidth / 2.f - 100.f, 300.f }, "", objects::roboto, 40, color::colors[static_cast<int>(color::ColorsName::MediumWhite)]);

		objects::verText = label::init(vec::Vector2{ externs::screenWidth / 3.0f, 10.0f }, "Gil climb", objects::roboto, 100, color::colors[static_cast<int>(color::ColorsName::RedNapthol)]);
	}

	static void update()
	{
		delta::updateDeltaT();

		scenes::currentScene = scenes::nextScene;

		switch (scenes::currentScene)
		{
		case game::Scene::Playing:
		{
			wasPausedPressed = isPausedPressed;
			isPausedPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);

			if (isPausedPressed && !wasPausedPressed)
			{
				isPaused = !isPaused;

				std::cout << objects::cameraOffset;
			}

			if (isPaused)
			{
				return;
			}

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
			button::update(objects::window, objects::rules);
			button::update(objects::window, objects::exit);

			if (objects::play.clicked)
			{
				scenes::nextScene = Scene::Playing;
				isPaused = false;
				sound::init();
			}

			if (objects::rules.clicked)
			{
				scenes::nextScene = Scene::Rules;
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
			button::update(objects::window, objects::back);

			if (objects::back.clicked)
			{
				scenes::nextScene = Scene::MainMenu;
			}

			break;
		}
		case game::Scene::Rules:
		{
			button::update(objects::window, objects::back);

			if (objects::back.clicked)
			{
				scenes::nextScene = Scene::MainMenu;
			}

			break;
		}
		case game::Scene::Death:
		{

			std::string scoreStr = "Final Score: " + std::to_string(static_cast<int>(objects::car.score));

			objects::deathScoreLabel.text = scoreStr;
			objects::deathScoreLabel.pos = { externs::screenWidth / 2.f - 170.f, 300.f };

			button::update(objects::window, objects::deathMenu);
			button::update(objects::window, objects::deathExit);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
			{
				car::reset(objects::car, { externs::screenWidth, externs::screenHeight / 3.0f });
				scenes::nextScene = Scene::Playing;
				sound::init();
			}

			if (objects::deathMenu.clicked)
			{
				car::reset(objects::car, { externs::screenWidth, externs::screenHeight / 3.0f });
				scenes::nextScene = Scene::MainMenu;
			}

			if (objects::deathExit.clicked)
			{
				objects::window.close();
			}

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

			if (isPaused)
			{
				sf::RectangleShape screen;
				screen.setOrigin({ objects::car.transform.position.x,-objects::car.transform.position.y });
				screen.setPosition({ objects::car.transform.position.x,objects::car.transform.position.y });
				screen.setSize({ 9999.0f,9999.0f });

				car::drawScore(objects::car, objects::window, objects::roboto);

				screen.setFillColor({ 128,128,128,128 });

				objects::window.draw(screen);
			}

			sf::View currentView = objects::window.getView();

			objects::window.setView(objects::window.getDefaultView());

			car::drawScore(objects::car, objects::window, objects::roboto);

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
			button::draw(objects::window, objects::rules, objects::roboto);

			break;
		}
		case game::Scene::Credits:
		{
			button::draw(objects::window, objects::back, objects::roboto);

			for (int i = 0; i < objects::maxCreditsText; i++)
			{
				label::draw(objects::creditsText[i], objects::window);
			}

			break;
		}
		case game::Scene::Rules:
		{
			button::draw(objects::window, objects::back, objects::roboto);

			for (int i = 0; i < objects::maxRulesText; i++)
			{
				label::draw(objects::rulesText[i], objects::window);
			}

			break;
		}
		case game::Scene::Death:
		{

			objects::window.setView(objects::window.getDefaultView());

			objects::window.clear(sf::Color(50, 0, 0));

			label::draw(objects::deathTitle, objects::window);
			label::draw(objects::deathScoreLabel, objects::window);
			label::draw(objects::restartLabel, objects::window);

			button::draw(objects::window, objects::deathMenu, objects::roboto);
			button::draw(objects::window, objects::deathExit, objects::roboto);

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
				if (isDeathBox)
				{
					if (mth::Max(p1.y, p2.y) > points[k].y && (p1.x > points[k].x && p2.x < points[k].x))
					{
						return true;
					}

				}
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
			if (externs::deathSound.getStatus() != sf::SoundSource::Status::Playing)
			{
				externs::deathSound.play();
				scenes::nextScene = Scene::Death;
			}

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

		objects::camera.setSize({ mth::Min(objects::camera.getSize().x,1900.0f),mth::Min(objects::camera.getSize().y,1069.0f) });
		objects::camera.setSize({ mth::Max(objects::camera.getSize().x,300.0f),mth::Max(objects::camera.getSize().y,169.0f) });

		mth::Clamp(objects::cameraOffset, -269.0f, 425.0f);
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

			if (!externs::trickBuffer.loadFromFile("res/sound/trick.wav"))
			{
				std::cout << "Error: trick sound not found";
			}

			externs::trickSound.setVolume(10.0f);

			if (!externs::deathBuffer.loadFromFile("res/sound/death.wav"))
			{
				std::cout << "Error: death sound not found";
			}
			externs::deathSound.setBuffer(externs::deathBuffer);

			externs::deathSound.setVolume(10.0f);

			if (!externs::clapBuffer.loadFromFile("res/sound/clap.wav"))
			{
				std::cout << "Error: clap sound not found";
			}
			externs::clapSound.setBuffer(externs::clapBuffer);

			externs::clapSound.setVolume(10.0f);

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
