#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct Button
{
    // Constructor to initialize the text with a default font
    Button();

    sf::RectangleShape shape;
    sf::Text label; // No optional, just the raw text object
    sf::Vector2f anchorPosition;
    sf::Color idleColor;
    sf::Color hoverColor;
    bool isHovered;
};

// Summary
// Initializes a button with a relative anchor position (0.0-1.0), size, text, and font.
void Init(Button& button, sf::Vector2f anchor, sf::Vector2f size, const std::string& text, const sf::Font& font, const sf::RenderWindow& window);

// Summary
// Recalculates the button's pixel position based on the current window size and its anchor.
void UpdateLayout(Button& button, const sf::RenderWindow& window);

// Summary
// Updates the button's state (hover color) based on the mouse position.
// Returns true if the button was clicked this frame.
bool UpdateLogic(Button& button, const sf::RenderWindow& window);

// Summary
// Draws the button shape and text to the window.
void Draw(const Button& button, sf::RenderWindow& window);