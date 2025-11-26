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
			vec::Vector2 offset;

			float maxSuspension;
			float stiffness;
			float damping;

			float lastCompression;
			bool isGrounded;
		};
	}

	struct Car
	{
		std::vector<wheel::Wheel> wheels;

		rigidbody::Rigidbody rigidBody;
		trans::Transform transform;

		shape::Rectangle collision;
	};

	Car init();
	void update(Car& car);
	void draw(Car car, sf::RenderWindow& window);
}