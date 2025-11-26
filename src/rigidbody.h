#pragma once

#include "transform.h"
#include "globals.h"

namespace rigidbody
{
	struct  Rigidbody
	{
		float mass = 100.0f;

		vec::Vector2 velocity = { 0.0f, 0.0f };
		vec::Vector2 force = { 0.0f, 0.0f };

		float angularVelocity = 0.0f;
		float torque = 0.0f;
	};

	void AddForce(Rigidbody& rigidbody, vec::Vector2 force);

	void AddTorque(Rigidbody& rigidbody, float torque);

	void Update(Rigidbody& rigidbody, trans::Transform& transform);
}