#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "rigidbody.h"
#include "transform.h"
#include "regular.h"
#include "collision.h"

namespace car
{
	namespace wheel
	{
		struct Wheel
		{
			trans::Transform transform;
			rigidbody::Rigidbody rigidBody;
			shape::Circle collision;

			rigidbody::Spring suspension;

			float isGroundedTimerLimit = 0.75f;
			float isGroundedTimer = isGroundedTimerLimit;
			bool isGrounded = false;

			vec::Vector2 offset;
			vec::Vector2 anchorOffset;

			sf::Sprite sprite{ externs::placeholder };
		};
	}

	struct Car
	{
		trans::Transform transform;
		rigidbody::Rigidbody rigidBody;

		shape::Rectangle collision;
		shape::Rectangle deathCollision;

		std::vector<wheel::Wheel> wheels;

		float score = 0.0f;

		float distanceScore = 0.0f;
		float airScore = 0.0f;
		float airTime = 0.0f;

		int airTricks = 0;
		float liftAngle = 0;

		bool isAlive = false;

		sf::Keyboard::Key accelerateKey;
		sf::Keyboard::Key brakeKey;

		sf::Sprite driverSprite{ externs::placeholder };
		vec::Vector2 driverSeatOffset;

		sf::Sprite sprite{ externs::placeholder };
	};

	Car init();
	void update(Car& car);
	void draw(Car& car, sf::RenderWindow& window);
	void reset(Car& car, vec::Vector2 position = { externs::screenWidth,externs::screenHeight / 4.0f });
}