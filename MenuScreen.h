#ifndef MENUSCREEN_H
#define MENUSCREEN_H
#pragma once
#include <SFML/Graphics.hpp>

class MenuScreen {
public:
    virtual ~MenuScreen() = default;
    virtual void updateHover(sf::Vector2f mousePos) = 0;
    virtual int handleClick(sf::Vector2f mousePos) = 0;
    virtual void draw(sf::RenderTexture& target) = 0;
};
#endif // MENUSCREEN_H
