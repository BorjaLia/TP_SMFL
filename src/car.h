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

			float isGroundedTimerLimit = 0.75f;
			float isGroundedTimer = isGroundedTimerLimit;
			bool isGrounded = false;
			vec::Vector2 offset;
			vec::Vector2 anchorOffset;

			float restLength = 0.0f;
			float stiffness = 0.0f;
			float damping = 0.0f;
			float maxSuspension = 0.0f;
		};
	}

	struct Car
	{
		trans::Transform transform;
		rigidbody::Rigidbody rigidBody;

		shape::Rectangle collision;
		
		std::vector<wheel::Wheel> wheels;
	};

	Car init();
	void update(Car& car);
	void draw(Car car, sf::RenderWindow& window);
	void reset(Car& car, vec::Vector2 position);
}