#pragma once

#include <SFML/Graphics.hpp>

#include "rigidbody.h"
#include "transform.h"
#include "regular.h"

namespace car
{
	struct Car
	{
		rigidbody::Rigidbody rigidBody;
		trans::Transform transform;

		shape::Rectangle collision;
	};

	Car init();
	void update(Car& car);
	void draw(Car car, sf::RenderWindow& window);
}