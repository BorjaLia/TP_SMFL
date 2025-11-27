#include "car.h"
#include <SFML/System/Angle.hpp> 

#include "globals.h"

namespace car
{
	static void manageInput(Car& car);

	Car init()
	{
		Car car;
		wheel::Wheel wheel1;
		wheel::Wheel wheel2;

		car.rigidBody.mass = 100.0f;
		car.rigidBody.velocity = { 0.0f, 0.0f };
		car.rigidBody.angularVelocity = 0.0f;
		car.rigidBody.torque = 0.0f;

		car.transform.rotation = 0.0f;
		car.transform.scale = { 0.0f, 0.0f };
		car.transform.position = { (externs::screenWidth / 2.0f) , (externs::screenHeight / 2.0f) };

		car.collision.pos = car.transform.position;
		car.collision.size = { 50.0f,25.0f };

		wheel1.maxSuspension = 50.0f;
		wheel1.stiffness = 800.0f;
		wheel1.damping = 30.0f;

		wheel1.lastCompression = 0.0f;
		wheel1.isGrounded = false;

		wheel2.maxSuspension = 50.0f;
		wheel2.stiffness = 800.0f;
		wheel2.damping = 30.0f;

		wheel2.lastCompression = 0.0f;
		wheel2.isGrounded = false;

		car.wheels.push_back(wheel1);
		car.wheels.push_back(wheel2);

		car.wheels[0].offset = { ((car.transform.position.x + car.collision.size.x) - 20), car.transform.position.y + car.collision.size.y };
		car.wheels[1].offset = { ((car.transform.position.x + car.collision.size.x) - 20) - 30, car.transform.position.y + car.collision.size.y };

		return car;
	}

	void update(Car& car)
	{
		manageInput(car);

		rigidbody::AddForce(car.rigidBody, { 0.0f, globals::gravity * car.rigidBody.mass });

		car.wheels[0].offset = { ((car.transform.position.x + car.collision.size.x) - 20), car.transform.position.y + car.collision.size.y };
		car.wheels[1].offset = { ((car.transform.position.x + car.collision.size.x) - 20) - 30, car.transform.position.y + car.collision.size.y };

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

		sf::Angle angle = sf::degrees(car.transform.rotation);

		rectangle.rotate(angle);

		sf::CircleShape circle{ 10, 10 };
		sf::CircleShape circle2{ 10, 10 };

		circle.setPosition({ car.wheels[0].offset.x, car.wheels[0].offset.y });
		circle2.setPosition({ car.wheels[1].offset.x, car.wheels[1].offset.y });

		window.draw(rectangle);
		window.draw(circle);
		window.draw(circle2);
	}

	void manageInput(Car& car)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && car.wheels[0].isGrounded)
		{
			rigidbody::AddForce(car.rigidBody, { -20.0f * car.rigidBody.mass, 0.0f });
			rigidbody::AddTorque(car.rigidBody, 10.0f);
			std::cout << "a";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && car.wheels[0].isGrounded)
		{
			rigidbody::AddForce(car.rigidBody, { 20.0f * car.rigidBody.mass, 0.0f });
			rigidbody::AddTorque(car.rigidBody, -10.0f);
			std::cout << "d";

		}
	}
}