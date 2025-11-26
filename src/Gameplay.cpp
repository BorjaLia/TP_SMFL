#include "Gameplay.h"

namespace Gameplay
{
    // Initializes resources required for the Gameplay
    void Init()
    {
    }

    // Processes input and logic for the Gameplay
    Scene Update(sf::RenderWindow& window, float deltaTime)
    {
        deltaTime++;
        return Scene::Gameplay;
    }

    // Renders the Gameplay to the screen
    void Draw(sf::RenderWindow& window)
    {
    }
}