#pragma once

#include "rigidbody.h"
#include "transform.h"

namespace car
{
	struct Car
	{
		Rigibody rigidBody;
		Transform transform;

		vec::Vector2 pos;
		vec::Vector2 acceleration;
	};

	Car init();
	void update(Car& car);
	void draw(Car car, sf::RenderWindow& window);
}