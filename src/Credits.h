#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

namespace Credits
{
    // Initializes resources required for the Credits
    void Init();

    // Processes input and logic for the Credits
    Scene Update(sf::RenderWindow& window, float deltaTime);

    // Renders the Credits to the screen
    void Draw(sf::RenderWindow& window);
}