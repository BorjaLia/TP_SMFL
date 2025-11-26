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
		transform.rotation += rigidbody.angularVelocity * externs::deltaT;

		rigidbody.force = { 0.0f, 0.0f };
		rigidbody.torque = 0.0f;
	}
}