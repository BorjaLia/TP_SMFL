#include "car.h"

#include "globals.h"

namespace car
{
	Car init()
	{
		Car car;

		car.rigidBody.mass = 100.0f;
		car.rigidBody.velocity = { 0.0f, 0.0f };
		car.rigidBody.angularVelocity = 0.0f;
		car.rigidBody.torque = 0.0f;

		car.transform.rotation = 0.0f;
		car.transform.scale = { 0.0f, 0.0f };
		car.transform.position = { (externs::screenWidth / 2.0f) , (externs::screenHeight / 2.0f) };

		car.collision.pos = car.transform.position;
		car.collision.size = { 50.0f,25.0f };

		return car;
	}

	void update(Car& car)
	{
		rigidbody::AddForce(car.rigidBody, { 0.0f,globals::gravity * car.rigidBody.mass });

		//car.rigidBody.velocity.x += car.acceleration.x * externs::deltaT;
		//car.rigidBody.velocity.y += car.acceleration.y * externs::deltaT;

		//car.pos.x += car.rigidBody.velocity.x * externs::deltaT;
		//car.pos.y += car.rigidBody.velocity.y * externs::deltaT;

		//car.transform.position = car.pos;

		rigidbody::Update(car.rigidBody, car.transform);
	}

	void draw(Car car, sf::RenderWindow& window)
	{
		sf::RectangleShape rectangle({ car.collision.size.x,car.collision.size.y });

		rectangle.setPosition({ car.transform.position.x, car.transform.position.y });
		window.draw(rectangle);
	}
}