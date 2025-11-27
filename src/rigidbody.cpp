#include "rigidbody.h"

namespace rigidbody
{
	void AddForce(Rigidbody& rigidbody, vec::Vector2 force)
	{
		rigidbody.force += force;
	}

	void AddTorque(Rigidbody& rigidbody, float torque)
	{
		rigidbody.torque += torque;
	}

	void AddForceAtPosition(Rigidbody& rigidbody,vec::Vector2 worldForce, vec::Vector2 worldPoint, vec::Vector2 center)
	{
		AddForce(rigidbody,worldForce);

		float rx = worldPoint.x - center.x;
		float ry = worldPoint.y - center.y;

		float addedTorque = (rx * worldForce.y) - (ry * worldForce.x);

		AddTorque(rigidbody,addedTorque);
	}

	void ApplyCollisionImpulse(Rigidbody& rigidbody,vec::Vector2 hitPoint, vec::Vector2 center, float stiffness)
	{
		vec::Vector2 reactionForce;
		reactionForce.x = -rigidbody.velocity.x * stiffness * rigidbody.mass;
		reactionForce.y = -rigidbody.velocity.y * stiffness * rigidbody.mass;

		AddForceAtPosition(rigidbody,reactionForce, hitPoint, center);
	}

	void Update(Rigidbody& rigidbody, trans::Transform& transform)
	{
		if (rigidbody.mass == 0.0f)
		{
			return;
		}

		vec::Vector2 acceleration = rigidbody.force * (1.0f / rigidbody.mass);

		rigidbody.velocity += acceleration * externs::deltaT;

		transform.position += rigidbody.velocity * externs::deltaT;

		float angularAccel = rigidbody.torque * (1.0f / rigidbody.mass);

		rigidbody.angularVelocity += angularAccel * externs::deltaT;
		rigidbody.angularVelocity *= (1.0f / (1.0f + rigidbody.angularDamping * externs::deltaT));
		
		transform.rotation += rigidbody.angularVelocity * externs::deltaT;

		rigidbody.force = { 0.0f, 0.0f };
		rigidbody.torque = 0.0f;
	}
}