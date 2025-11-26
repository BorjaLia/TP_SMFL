#pragma once
#include <SFML/Graphics.hpp>

struct Entity
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f size;
};

// Summary
// Initializes the entity with a starting position and size.
// Velocity is initialized to (0, 0).
void Init(Entity& entity, sf::Vector2f position, sf::Vector2f size);

// Summary
// Updates the position of the entity based on its current velocity and the time step.
void UpdatePhysics(Entity& entity, float deltaTime);

// Summary
// Checks for an AABB (Axis-Aligned Bounding Box) collision between two entities.
// Returns true if they are overlapping.
bool CheckCollision(const Entity& a, const Entity& b);