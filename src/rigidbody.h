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

		float angularDamping = 2.0f;
	};

	struct Spring
	{
		vec::Vector2 localAttachment = { 0.0f, 0.0f };
		vec::Vector2 anchor = { 0.0f, 0.0f };

		vec::Vector2 stiffness = { 10.0f, 10.0f };

		vec::Vector2 damping = { 0.5f, 0.5f };

		float restLength = 0.0f;
		float maxDistance = 100.0f;

		bool useMaxDistance = true;
	};

	void addForce(Rigidbody& rigidbody, vec::Vector2 force);

	void addTorque(Rigidbody& rigidbody, float torque);

	void addForceAtPosition(Rigidbody& rigidbody, vec::Vector2 worldForce, vec::Vector2 worldPoint, vec::Vector2 center);

	void applyCollisionImpulse(Rigidbody& rigidbody, vec::Vector2 hitPoint, vec::Vector2 center, float stiffness);

	vec::Vector2 applySpring(Rigidbody& rigidBody, trans::Transform& transform, Spring& spring, vec::Vector2 anchorVelocity, float referenceAngle);

	void update(Rigidbody& rigidbody, trans::Transform& transform, std::vector<Spring>& springs);
	
	void update(Rigidbody& rigidbody, trans::Transform& transform);
}