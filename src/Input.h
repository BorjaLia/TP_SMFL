#pragma once
#include <SFML/Graphics.hpp>

namespace Input
{
    // Summary
    // Checks WASD and Arrow keys to return a normalized direction vector.
    // X will be -1 (Left), 0 (None), or 1 (Right).
    // Y will be -1 (Up), 0 (None), or 1 (Down).
    sf::Vector2f GetMovementAxis();

    // Summary
    // Checks if a generic "Action" button is pressed (e.g., Spacebar or Enter).
    bool IsActionPressed();

    // Summary
    // Returns the current mouse position relative to the window.
    sf::Vector2i GetMousePosition(const sf::RenderWindow& window);
}