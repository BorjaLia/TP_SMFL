#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

namespace Gameplay
{
    // Initializes resources required for the Gameplay
    void Init();

    // Processes input and logic for the Gameplay
    Scene Update(sf::RenderWindow& window, float deltaTime);

    // Renders the Gameplay to the screen
    void Draw(sf::RenderWindow& window);
}