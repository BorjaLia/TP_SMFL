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

		float wheelMass = 10.0f;
		float suspensionStiffness = 1000.0f;
		float suspensionDamping = 25.0f;
		float wheelRadius = 10.0f;

		wheel1.collision.radius = wheelRadius;
		wheel1.rigidBody.mass = wheelMass;
		wheel1.isGrounded = false;
		wheel1.stiffness = suspensionStiffness;
		wheel1.damping = suspensionDamping;
		wheel1.restLength = 20.0f;

		wheel1.anchorOffset = { -halfWidth + 10.0f, 2 * halfHeight };

		wheel1.transform.position = car.transform.position + wheel1.anchorOffset;
		wheel1.transform.position.y += wheel1.restLength;

		wheel2.collision.radius = wheelRadius;
		wheel2.rigidBody.mass = wheelMass;
		wheel2.isGrounded = false;
		wheel2.stiffness = suspensionStiffness;
		wheel2.damping = suspensionDamping;
		wheel2.restLength = 20.0f;

		wheel2.anchorOffset = { halfWidth - 10.0f, 2 * halfHeight };

		wheel2.transform.position = car.transform.position + wheel2.anchorOffset;
		wheel2.transform.position.y += wheel2.restLength;

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

		sf::Angle angle = sf::degrees(car.transform.rotation);
		rectangle.setRotation(angle);

		window.draw(rectangle);

		for (int i = 0; i < car.wheels.size(); i++)
		{
			sf::CircleShape circle(car.wheels[i].collision.radius);

			circle.setOrigin({ car.wheels[i].collision.radius, car.wheels[i].collision.radius });

			circle.setPosition({ car.wheels[i].transform.position.x, car.wheels[i].transform.position.y });

			circle.setRotation(sf::degrees(car.wheels[i].transform.rotation));

			window.draw(circle);
		}

		sf::CircleShape com(5.0f);
		com.setOrigin({ 5.0f, 5.0f });
		com.setFillColor(sf::Color::Magenta);
		com.setPosition({ car.transform.position.x, car.transform.position.y });
		window.draw(com);

		sf::RectangleShape normalRectangle({ car.collision.size.x,5.0f });

		normalRectangle.setPosition({ car.transform.position.x, car.transform.position.y });

		sf::Angle normalAngle = sf::degrees(car.transform.rotation + 90);
		normalRectangle.setRotation(normalAngle);

		window.draw(normalRectangle);
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
			restPos.y += car.wheels[i].restLength;

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

		if (!car.wheels[0].isGrounded && !car.wheels[1].isGrounded)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				rigidbody::AddForce(car.rigidBody, { -20.0f * car.rigidBody.mass, 0.0f });
				rigidbody::AddTorque(car.rigidBody, 100000.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				rigidbody::AddForce(car.rigidBody, { 20.0f * car.rigidBody.mass, 0.0f });
				rigidbody::AddTorque(car.rigidBody, -100000.0f);
			}
		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				rigidbody::AddForce(car.rigidBody, { -120.0f * car.rigidBody.mass, 0.0f });
				rigidbody::AddTorque(car.rigidBody, 50000.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				rigidbody::AddForce(car.rigidBody, { 120.0f * car.rigidBody.mass, 0.0f });
				rigidbody::AddTorque(car.rigidBody, -50000.0f);
			}
		}
	}

	static void updateSuspension(Car& car)
	{
		for (int i = 0; i < car.wheels.size(); i++)
		{
			vec::Vector2 mountPosLocal = car.wheels[i].anchorOffset.rotatedDegree(car.transform.rotation);
			vec::Vector2 mountPosWorld = car.transform.position + mountPosLocal;

			vec::Vector2 diff = mountPosWorld - car.wheels[i].transform.position;

			vec::Vector2 localDiff = diff.rotatedDegree(-car.transform.rotation);

			vec::Vector2 mountPos = car.transform.position + car.wheels[i].anchorOffset.rotatedDegree(car.transform.rotation);
			vec::Vector2 springVector = car.wheels[i].transform.position - mountPos;

			float maxLen = car.wheels[i].restLength * 2.0f;

			if (springVector.magnitude() > maxLen)
			{
				springVector.normalize();

				springVector = springVector * maxLen;

				car.wheels[i].transform.position = mountPos + springVector;

				car.wheels[i].rigidBody.velocity = car.rigidBody.velocity;

				diff = mountPos - car.wheels[i].transform.position;
				localDiff = diff.rotatedDegree(-car.transform.rotation);
			}
			vec::Vector2 driftForceLocal = { localDiff.x * 300.0f, 0.0f };

			float currentLength = localDiff.y;
			float compression = currentLength - car.wheels[i].restLength;

			float springForceY = compression * car.wheels[i].stiffness;

			vec::Vector2 carVelAtMount = car.rigidBody.velocity;
			float relVelY = (carVelAtMount.y - car.wheels[i].rigidBody.velocity.y);
			float dampingForceY = relVelY * car.wheels[i].damping;

			vec::Vector2 suspensionForceLocal = { 0.0f, springForceY + dampingForceY };

			vec::Vector2 totalForceLocal = driftForceLocal + suspensionForceLocal;
			vec::Vector2 totalForceWorld = totalForceLocal.rotatedDegree(car.transform.rotation);

			rigidbody::AddForceAtPosition(car.rigidBody, totalForceWorld * -1.0f, mountPosWorld, car.transform.position);

			rigidbody::AddForce(car.wheels[i].rigidBody, totalForceWorld);
		}
	}
}