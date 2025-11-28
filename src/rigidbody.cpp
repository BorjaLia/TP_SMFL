#include "rigidbody.h"

namespace rigidbody
{
	void addForce(Rigidbody& rigidbody, vec::Vector2 force)
	{
		rigidbody.force += force;
	}

	void addTorque(Rigidbody& rigidbody, float torque)
	{
		rigidbody.torque += torque;
	}

	void addForceAtPosition(Rigidbody& rigidbody,vec::Vector2 worldForce, vec::Vector2 worldPoint, vec::Vector2 center)
	{
		addForce(rigidbody,worldForce);

		float rx = worldPoint.x - center.x;
		float ry = worldPoint.y - center.y;

		float addedTorque = (rx * worldForce.y) - (ry * worldForce.x);

		addTorque(rigidbody,addedTorque);
	}

	void applyCollisionImpulse(Rigidbody& rigidbody,vec::Vector2 hitPoint, vec::Vector2 center, float stiffness)
	{
		vec::Vector2 reactionForce;
		reactionForce.x = -rigidbody.velocity.x * stiffness * rigidbody.mass;
		reactionForce.y = -rigidbody.velocity.y * stiffness * rigidbody.mass;

		addForceAtPosition(rigidbody,reactionForce, hitPoint, center);
	}

	vec::Vector2 applySpring(Rigidbody& rigidBody, trans::Transform& transform, Spring& spring, vec::Vector2 anchorVelocity, float referenceAngle)
	{
		vec::Vector2 arm = spring.localAttachment.rotated(transform.rotation);
		vec::Vector2 attachmentWorld = transform.position + arm;

		vec::Vector2 diff = attachmentWorld - spring.anchor;
		float dist = diff.magnitude();

		if (spring.useMaxDistance && dist > spring.maxDistance)
		{
			if (dist > 0.0001f)
			{
				vec::Vector2 dir = diff.normalized();
				vec::Vector2 targetPos = spring.anchor + (dir * spring.maxDistance);
				vec::Vector2 correction = targetPos - attachmentWorld;
				transform.position += correction;

				attachmentWorld = transform.position + arm;
				diff = attachmentWorld - spring.anchor;

				vec::Vector2 relativeVel = rigidBody.velocity - anchorVelocity;
				float velProj = relativeVel * dir;
				if (velProj > 0)
				{
					rigidBody.velocity -= dir * velProj;
				}
			}
		}

		vec::Vector2 localDiff = diff.rotated(-referenceAngle);
		vec::Vector2 localForce = { 0.0f, 0.0f };

		localForce.x = -localDiff.x * spring.stiffness.x;

		float displacementY = localDiff.y - spring.restLength;
		localForce.y = -displacementY * spring.stiffness.y;

		vec::Vector2 pointVel = rigidBody.velocity;
		pointVel.x += -rigidBody.angularVelocity * arm.y;
		pointVel.y += rigidBody.angularVelocity * arm.x;

		vec::Vector2 relativeVelocity = pointVel - anchorVelocity;

		vec::Vector2 localVel = relativeVelocity.rotated(-referenceAngle);

		vec::Vector2 localDamping;
		localDamping.x = -localVel.x * spring.damping.x;
		localDamping.y = -localVel.y * spring.damping.y;

		vec::Vector2 totalLocalForce = localForce + localDamping;

		vec::Vector2 worldForce = totalLocalForce.rotated(referenceAngle);

		worldForce.clamp(20000.0f);

		addForceAtPosition(rigidBody, worldForce, attachmentWorld, transform.position);

		return worldForce;
	}

	void update(Rigidbody& rigidbody, trans::Transform& transform, std::vector<Spring>& springs)
	{
		for (int i = 0; i < (int)springs.size(); i++)
		{
			applySpring(rigidbody, transform, springs[i], { 0.0f, 0.0f }, transform.rotation);
		}

		update(rigidbody, transform);
	}

	void update(Rigidbody& rigidbody, trans::Transform& transform)
	{
		if (rigidbody.mass == 0.0f)
		{
			return;
		}

		vec::Vector2 acceleration = rigidbody.force * (1.0f / rigidbody.mass);

		rigidbody.velocity += acceleration * externs::deltaT;

		transform.position += rigidbody.velocity * externs::deltaT;

		float inertia = rigidbody.mass * (100.0f * 100.0f + 50.0f * 50.0f) / 12.0f;
		float angularAccel = rigidbody.torque * (1.0f / inertia);

		rigidbody.angularVelocity += angularAccel * externs::deltaT;
		rigidbody.angularVelocity *= (1.0f / (1.0f + rigidbody.angularDamping * externs::deltaT));
		
		transform.rotation += rigidbody.angularVelocity * externs::deltaT;

		rigidbody.force = { 0.0f, 0.0f };
		rigidbody.torque = 0.0f;
	}
}