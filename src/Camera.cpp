#include "Camera.h"

// Summary
// Initializes the camera with a specific center position and view size.
void Init(Camera& camera, sf::Vector2f initialPosition, sf::Vector2f initialSize)
{
    camera.position = initialPosition;
    camera.size = initialSize;
    UpdateView(camera);
}

// Summary
// Syncs the internal SFML view with the current position and size of the Camera struct.
void UpdateView(Camera& camera)
{
    camera.view.setCenter(camera.position);
    camera.view.setSize(camera.size);
}

// Summary
// Sets this camera as the active view for the given window.
void ApplyToWindow(const Camera& camera, sf::RenderWindow& window)
{
    window.setView(camera.view);
}

// Summary
// Converts a pixel coordinate (like mouse position) into world coordinates based on this camera.
sf::Vector2f ScreenToWorld(const Camera& camera, const sf::RenderWindow& window, sf::Vector2i screenPos)
{
    // mapPixelToCoords automatically accounts for the currently set view, 
    // but to be safe we explicitly pass the camera's view.
    return window.mapPixelToCoords(screenPos, camera.view);
}