#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "MainMenu.h"
#include "Gameplay.h"
#include "Credits.h"
#include <iostream>

// Summary
// Entry point of the application.
int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const char* title = "SFML Framework Project";

    // FIX: Use direct initialization () instead of copy initialization =
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight) }), title);

    sf::Clock clock;
    bool isRunning = true;
    Scene currentScene = Scene::MainMenu;

    // Pass window to MainMenu::Init so buttons can calculate anchors
    MainMenu::Init(window);
    Gameplay::Init();
    Credits::Init();

    while (window.isOpen() && isRunning)
    {
        sf::Time dtObj = clock.restart();
        float deltaTime = dtObj.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                isRunning = false;
            }
        }

        switch (currentScene)
        {
        case Scene::MainMenu:
            currentScene = MainMenu::Update(window, deltaTime);
            break;

        case Scene::Gameplay:
            currentScene = Gameplay::Update(window, deltaTime);
            break;

        case Scene::Credits:
            currentScene = Credits::Update(window, deltaTime);
            break;

        default:
            break;
        }

        window.clear(sf::Color::Black);

        switch (currentScene)
        {
        case Scene::MainMenu:
            MainMenu::Draw(window);
            break;

        case Scene::Gameplay:
            Gameplay::Draw(window);
            break;

        case Scene::Credits:
            Credits::Draw(window);
            break;

        default:
            break;
        }

        window.display();
    }

    return 0;
}