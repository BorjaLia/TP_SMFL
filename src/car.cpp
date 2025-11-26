#include "car.h"

#include "globals.h"

namespace car
{
	Car init()
	{
		Car car;

		car.rigidBody.mass = 1.0f;
		car.rigidBody.velocity = { 3.0f, 0.0f };
		car.rigidBody.angularVelocity = 0.0f;
		car.rigidBody.torque = 0.0f;

		car.transform.rotation = 0.0f;
		car.transform.scale = { 0.0f, 0.0f };
		car.transform.translation = { 0.0f, 0.0f };

		car.pos = { (externs::screenWidth / 2.0f) , (externs::screenHeight / 2.0f) };

		return car;
	}

	void update(Car& car)
	{
		car.rigidBody.velocity.y += globals::gravity * externs::deltaT;

		car.rigidBody.velocity.x += car.acceleration.x * externs::deltaT;
		car.rigidBody.velocity.y += car.acceleration.y * externs::deltaT;

		car.pos.x += car.rigidBody.velocity.x * externs::deltaT;
		car.pos.y += car.rigidBody.velocity.y * externs::deltaT;

		car.transform.translation = car.pos;
	}

	void draw(Car car, sf::RenderWindow& window)
	{
		sf::RectangleShape rectangle({ 20.0f,15.0f });

		rectangle.setPosition({ car.pos.x, car.pos.y });
		window.draw(rectangle);
	}
}