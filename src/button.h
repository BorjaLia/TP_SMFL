#pragma once

#include <SFML/Graphics.hpp>

#include "LLM_math.h"

#include <string>

namespace button
{
    enum class State
    {
        Normal,
        Hover,
        Pressed
    };

    struct Button
    {
        vec::Vector2 position;
        float width;
        float height;

        std::string text;
        State state;
        bool clicked = false;
    };

    Button init(float x, float y, float width, float height, std::string& text);
    bool isMouseOnButton(sf::RenderWindow& window, Button& button);
    void update(sf::RenderWindow& window, Button& button);
    void draw(sf::RenderWindow& window, Button button, sf::Font roboto);

}