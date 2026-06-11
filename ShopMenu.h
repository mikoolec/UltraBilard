#ifndef SHOPMENU_H
#define SHOPMENU_H
#pragma once
#include "MenuScreen.h"

class ShopMenu : public MenuScreen {
private:
    sf::RectangleShape bg;
    sf::Texture texBtnNextNorm, texBtnNextHov;
    sf::Sprite btnNext;
    sf::Font font;
    sf::Text titleText;
public:
    ShopMenu(std::pair<int,int> res);
    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
};

#endif // SHOPMENU_H
