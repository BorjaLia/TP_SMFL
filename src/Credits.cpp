#include "Credits.h"

namespace Credits
{
    // Initializes resources required for the Credits
    void Init()
    {
    }

    // Processes input and logic for the Credits
    Scene Update(sf::RenderWindow& window, float deltaTime)
    {
        deltaTime++;
        return Scene::Credits;
    }

    // Renders the Credits to the screen
    void Draw(sf::RenderWindow& window)
    {
    }
}