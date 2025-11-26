#include "Entity.h"

// Summary
// Initializes the entity with a starting position and size.
void Init(Entity& entity, sf::Vector2f position, sf::Vector2f size)
{
    entity.position = position;
    entity.size = size;
    entity.velocity = sf::Vector2f(0.f, 0.f);
}

// Summary
// Updates the position of the entity based on its current velocity and the time step.
void UpdatePhysics(Entity& entity, float deltaTime)
{
    entity.position.x += entity.velocity.x * deltaTime;
    entity.position.y += entity.velocity.y * deltaTime;
}

// Summary
// Checks for an AABB (Axis-Aligned Bounding Box) collision between two entities.
bool CheckCollision(const Entity& a, const Entity& b)
{
    // AABB Collision logic:
    // Check if the boxes are NOT intersecting, then negate the result.

    // Check horizontal overlap
    bool collisionX = a.position.x < b.position.x + b.size.x &&
        a.position.x + a.size.x > b.position.x;

    // Check vertical overlap
    bool collisionY = a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y;

    return collisionX && collisionY;
}