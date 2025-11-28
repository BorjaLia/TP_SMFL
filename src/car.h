#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "rigidbody.h"
#include "transform.h"
#include "regular.h"

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
		};
	}

	struct Car
	{
		trans::Transform transform;
		rigidbody::Rigidbody rigidBody;

		shape::Rectangle collision;

		std::vector<wheel::Wheel> wheels;

		sf::Keyboard::Key accelerateKey;
		sf::Keyboard::Key brakeKey;
	};

	Car init();
	void update(Car& car);
	void draw(Car car, sf::RenderWindow& window);
	void reset(Car& car, vec::Vector2 position);
}