#include "Button.h"
#include "Input.h"

namespace
{
    // Summary
    // A static empty font used ONLY to satisfy the sf::Text constructor before the game starts.
    const sf::Font& GetDummyFont()
    {
        static sf::Font dummy;
        return dummy;
    }

    // Summary
    // Helper function to center text within the button shape.
    void CenterText(Button& button)
    {
        sf::FloatRect textBounds = button.label.getLocalBounds();

        button.label.setOrigin(sf::Vector2f(
            textBounds.position.x + textBounds.size.x / 2.0f,
            textBounds.position.y + textBounds.size.y / 2.0f
        ));

        button.label.setPosition(button.shape.getPosition());
    }
}

// Summary
// Default constructor. Initializes the label with a dummy font to prevent compiler errors.
Button::Button() : label(GetDummyFont())
{
    isHovered = false;
}

// Summary
// Initializes a button with a relative anchor position, size, text, and font.
void Init(Button& button, sf::Vector2f anchor, sf::Vector2f size, const std::string& text, const sf::Font& font, const sf::RenderWindow& window)
{
    button.anchorPosition = anchor;
    button.isHovered = false;

    // Visual settings
    button.idleColor = sf::Color(100, 100, 100);
    button.hoverColor = sf::Color(150, 150, 150);

    button.shape.setSize(size);
    button.shape.setFillColor(button.idleColor);

    button.shape.setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

    // Text settings
    // We overwrite the 'dummy' label with the real font and text here.
    button.label = sf::Text(font, text, 24);
    button.label.setFillColor(sf::Color::White);

    // Initial placement
    UpdateLayout(button, window);
}

// Summary
// Recalculates the button's pixel position based on the current window size.
void UpdateLayout(Button& button, const sf::RenderWindow& window)
{
    sf::Vector2u winSize = window.getSize();

    float x = static_cast<float>(winSize.x) * button.anchorPosition.x;
    float y = static_cast<float>(winSize.y) * button.anchorPosition.y;

    button.shape.setPosition(sf::Vector2f(x, y));

    // Re-center text after moving shape
    CenterText(button);
}

// Summary
// Updates the button's state and checks for clicks.
bool UpdateLogic(Button& button, const sf::RenderWindow& window)
{
    sf::Vector2i mousePos = Input::GetMousePosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

    bool wasClicked = false;

    if (button.shape.getGlobalBounds().contains(mouseWorldPos))
    {
        button.isHovered = true;
        button.shape.setFillColor(button.hoverColor);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            wasClicked = true;
        }
    }
    else
    {
        button.isHovered = false;
        button.shape.setFillColor(button.idleColor);
    }

    return wasClicked;
}

// Summary
// Draws the button shape and text to the window.
void Draw(const Button& button, sf::RenderWindow& window)
{
    window.draw(button.shape);
    window.draw(button.label);
}