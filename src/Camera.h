#pragma once
#include <SFML/Graphics.hpp>

struct Camera
{
    sf::Vector2f position;
    sf::Vector2f size;
    sf::View view;
};

// Summary
// Initializes the camera with a specific center position and view size.
void Init(Camera& camera, sf::Vector2f initialPosition, sf::Vector2f initialSize);

// Summary
// Syncs the internal SFML view with the current position and size of the Camera struct.
// Should be called whenever position or size changes.
void UpdateView(Camera& camera);

// Summary
// Sets this camera as the active view for the given window.
void ApplyToWindow(const Camera& camera, sf::RenderWindow& window);

// Summary
// Converts a pixel coordinate (like mouse position) into world coordinates based on this camera.
sf::Vector2f ScreenToWorld(const Camera& camera, const sf::RenderWindow& window, sf::Vector2i screenPos);