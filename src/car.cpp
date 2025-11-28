#include "car.h"
#include <SFML/System/Angle.hpp> 

#include "globals.h"

namespace car
{
	static void manageInput(Car& car);
	static void updateSuspension(Car& car);

	Car init()
	{
		Car car;

		car.accelerateKey = sf::Keyboard::Key::D;
		car.brakeKey = sf::Keyboard::Key::A;

		car.rigidBody.mass = 10.0f;
		car.rigidBody.velocity = { 0.0f, 0.0f };
		car.rigidBody.angularVelocity = 0.0f;
		car.rigidBody.torque = 0.0f;
		car.rigidBody.angularDamping = 5.0f;

		car.transform.rotation = 0.0f;
		car.transform.scale = { 1.0f, 1.0f };
		car.transform.position = { (externs::screenWidth / 2.0f) , (externs::screenHeight / 3.0f) };

		car.collision.pos = { 0.0f,0.0f };
		car.collision.size = { 100.0f, 50.0f };

		float halfWidth = car.collision.size.x / 2.0f;
		float halfHeight = car.collision.size.y / 2.0f;

		wheel::Wheel wheel1;
		wheel::Wheel wheel2;

		float wheelMass = 5.0f;
		float wheelRadius = 12.0f;
		float wheelRestLength = 25.0f;
		float wheelMaxDistance = 50.0f;

		vec::Vector2 stiffness = { 2500.0f, 75.0f };
		vec::Vector2 damping = { 100.0f, 50.0f };

		wheel1.collision.radius = wheelRadius;
		wheel1.rigidBody.mass = wheelMass;
		wheel1.isGrounded = false;

		wheel1.suspension.stiffness = stiffness;
		wheel1.suspension.damping = damping;
		wheel1.suspension.restLength = wheelRestLength;
		wheel1.suspension.maxDistance = wheelMaxDistance;

		wheel1.anchorOffset = { -halfWidth + 10.0f, halfHeight };

		wheel1.transform.position = car.transform.position + wheel1.anchorOffset;
		wheel1.transform.position.y += wheel1.suspension.restLength;

		wheel2.collision.radius = wheelRadius;
		wheel2.rigidBody.mass = wheelMass;
		wheel2.isGrounded = false;

		wheel2.suspension.stiffness = stiffness;
		wheel2.suspension.damping = damping;
		wheel2.suspension.restLength = wheelRestLength;
		wheel2.suspension.maxDistance = wheelMaxDistance;

		wheel2.anchorOffset = { halfWidth - 10.0f, halfHeight };

		wheel2.transform.position = car.transform.position + wheel2.anchorOffset;
		wheel2.transform.position.y += wheel2.suspension.restLength;

		car.wheels.push_back(wheel1);
		car.wheels.push_back(wheel2);

		return car;
	}

	void update(Car& car)
	{
		if (car.transform.position.y > externs::screenHeight)
		{
			reset(car, { car.transform.position.x,externs::screenHeight / 3.0f });
		}

		std::cout << car.transform.position << "\n";

		manageInput(car);

		vec::Vector2 gravity = { 0.0f, globals::gravity };

		rigidbody::AddForce(car.rigidBody, gravity * car.rigidBody.mass);

		for (int i = 0; i < car.wheels.size(); i++)
		{
			rigidbody::AddForce(car.wheels[i].rigidBody, gravity * car.wheels[i].rigidBody.mass);
		}

		updateSuspension(car);

		rigidbody::Update(car.rigidBody, car.transform);

		for (int i = 0; i < car.wheels.size(); i++)
		{
			rigidbody::Update(car.wheels[i].rigidBody, car.wheels[i].transform);
		}
	}

	void draw(Car car, sf::RenderWindow& window)
	{
		sf::RectangleShape rectangle({ car.collision.size.x, car.collision.size.y });

		rectangle.setOrigin({ car.collision.size.x / 2.0f, car.collision.size.y / 2.0f });
		rectangle.setPosition({ car.transform.position.x, car.transform.position.y });

		sf::Angle angle = sf::radians(car.transform.rotation);
		rectangle.setRotation(angle);

		window.draw(rectangle);

		for (int i = 0; i < car.wheels.size(); i++)
		{
			sf::CircleShape circle(car.wheels[i].collision.radius);

			circle.setOrigin({ car.wheels[i].collision.radius, car.wheels[i].collision.radius });

			circle.setPosition({ car.wheels[i].transform.position.x, car.wheels[i].transform.position.y });

			circle.setRotation(sf::degrees(car.wheels[i].transform.rotation));

			if (car.wheels[i].isGrounded)
			{
				circle.setFillColor(sf::Color::Cyan);
			}

			window.draw(circle);
		}

		sf::CircleShape com(5.0f);
		com.setOrigin({ 5.0f, 5.0f });
		com.setFillColor(sf::Color::Magenta);
		com.setPosition({ car.transform.position.x, car.transform.position.y });
		window.draw(com);
	}

	void reset(Car& car, vec::Vector2 position)
	{
		car.transform.position = position;
		car.transform.rotation = 0.0f;

		car.rigidBody.velocity = { 0.0f, 0.0f };
		car.rigidBody.angularVelocity = 0.0f;
		car.rigidBody.force = { 0.0f, 0.0f };
		car.rigidBody.torque = 0.0f;

		for (int i = 0; i < car.wheels.size(); i++)
		{
			vec::Vector2 restPos = position + car.wheels[i].anchorOffset;
			restPos.y += car.wheels[i].suspension.restLength;

			car.wheels[i].transform.position = restPos;
			car.wheels[i].transform.rotation = 0.0f;

			car.wheels[i].rigidBody.velocity = { 0.0f, 0.0f };
			car.wheels[i].rigidBody.force = { 0.0f, 0.0f };
			car.wheels[i].rigidBody.angularVelocity = 0.0f;
			car.wheels[i].rigidBody.torque = 0.0f;

			car.wheels[i].isGrounded = false;
		}
	}

	static void manageInput(Car& car)
	{
		float torquePower = 80000.0f;
		float drivePower = 200.0f;

		vec::Vector2 forwardDir = { 1.0f, 0.0f };
		forwardDir = forwardDir.rotated(car.transform.rotation);

		if (!car.wheels[0].isGrounded && !car.wheels[1].isGrounded)
		{
			if (sf::Keyboard::isKeyPressed(car.brakeKey))
			{
				rigidbody::AddTorque(car.rigidBody, torquePower * 0.5f);
			}
			if (sf::Keyboard::isKeyPressed(car.accelerateKey))
			{
				rigidbody::AddTorque(car.rigidBody, -torquePower * 0.5f);
			}
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(car.brakeKey))
			{
				rigidbody::AddForce(car.rigidBody, forwardDir * -drivePower * car.rigidBody.mass);
				rigidbody::AddTorque(car.rigidBody, torquePower);
			}
			if (sf::Keyboard::isKeyPressed(car.accelerateKey))
			{
				rigidbody::AddForce(car.rigidBody, forwardDir * drivePower * car.rigidBody.mass);
				rigidbody::AddTorque(car.rigidBody, -torquePower);
			}
		}
	}

	static void updateSuspension(Car& car)
	{
		for (int i = 0; i < (int)car.wheels.size(); i++)
		{
			wheel::Wheel& w = car.wheels[i];

			vec::Vector2 mountOffset = w.anchorOffset.rotated(car.transform.rotation);
			vec::Vector2 mountPosWorld = car.transform.position + mountOffset;

			w.suspension.anchor = mountPosWorld;

			vec::Vector2 anchorVel = car.rigidBody.velocity;
			anchorVel.x += -car.rigidBody.angularVelocity * mountOffset.y;
			anchorVel.y += car.rigidBody.angularVelocity * mountOffset.x;

			vec::Vector2 forceOnWheel = rigidbody::ApplySpring(w.rigidBody, w.transform, w.suspension, anchorVel, car.transform.rotation);

			rigidbody::AddForceAtPosition(car.rigidBody, forceOnWheel * -1.0f, mountPosWorld, car.transform.position);
		
		}

	}
}