#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

namespace MainMenu
{
    // Summary
    // Initializes resources required for the Main Menu, including loading fonts and setting up buttons.
    void Init(const sf::RenderWindow& window);

    // Summary
    // Processes input and logic for the Main Menu.
    Scene Update(sf::RenderWindow& window, float deltaTime);

    // Summary
    // Renders the Main Menu to the screen.
    void Draw(sf::RenderWindow& window);
}