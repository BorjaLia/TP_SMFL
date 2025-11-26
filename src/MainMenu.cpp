#include "MainMenu.h"
#include "Button.h"
#include <iostream>

namespace
{
    sf::Font menuFont;
    Button playButton;
    Button creditsButton;
    Button exitButton;
}

namespace MainMenu
{
    // Summary
    // Initializes resources required for the Main Menu.
    void Init(const sf::RenderWindow& window)
    {
        // Updated path to the new resource folder structure
        if (!menuFont.openFromFile("res/font/Roboto-Medium.ttf"))
        {
            std::cerr << "Error: Could not load res/font/Roboto-Medium.ttf" << std::endl;
        }

        sf::Vector2f buttonSize(200.f, 50.f);

        // Play Button anchored to the upper middle (0.5, 0.4)
        ::Init(playButton, sf::Vector2f(0.5f, 0.4f), buttonSize, "Play", menuFont, window);

        // Credits Button anchored to the center (0.5, 0.55)
        ::Init(creditsButton, sf::Vector2f(0.5f, 0.55f), buttonSize, "Credits", menuFont, window);

        // Exit Button anchored to the lower middle (0.5, 0.7)
        ::Init(exitButton, sf::Vector2f(0.5f, 0.7f), buttonSize, "Exit", menuFont, window);
    }

    // Summary
    // Processes input and logic for the Main Menu.
    Scene Update(sf::RenderWindow& window, float deltaTime)
    {
        // Play Button Logic
        if (::UpdateLogic(playButton, window))
        {
            return Scene::Gameplay;
        }

        // Credits Button Logic
        if (::UpdateLogic(creditsButton, window))
        {
            return Scene::Credits;
        }

        // Exit Button Logic
        if (::UpdateLogic(exitButton, window))
        {
            window.close();
        }

        return Scene::MainMenu;
    }

    // Summary
    // Renders the Main Menu to the screen.
    void Draw(sf::RenderWindow& window)
    {
        ::Draw(playButton, window);
        ::Draw(creditsButton, window);
        ::Draw(exitButton, window);
    }
}