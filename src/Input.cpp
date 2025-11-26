#include "Input.h"
#include <cmath> // For std::sqrt

namespace Input
{
    // Summary
    // Checks WASD and Arrow keys to return a normalized direction vector.
    sf::Vector2f GetMovementAxis()
    {
        sf::Vector2f axis(0.f, 0.f);

        // Horizontal Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            axis.x = -1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            // If both are pressed, they cancel out to 0
            axis.x += 1.f;
        }

        // Vertical Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            axis.y = -1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            axis.y += 1.f;
        }

        // Normalization (Fixes diagonal movement being faster)
        if (axis.x != 0.f || axis.y != 0.f)
        {
            // Calculate length (hypotenuse)
            float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);

            // Divide components by length to normalize
            axis.x /= length;
            axis.y /= length;
        }

        return axis;
    }

    // Summary
    // Checks if a generic "Action" button is pressed.
    bool IsActionPressed()
    {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
    }

    // Summary
    // Returns the current mouse position relative to the window.
    sf::Vector2i GetMousePosition(const sf::RenderWindow& window)
    {
        return sf::Mouse::getPosition(window);
    }
}