#ifndef SHOPMENU_H
#define SHOPMENU_H
#pragma once
#include "MenuScreen.h"
#include "Upgrades.h"
#include <vector>
#include <string>

// Stany samego sklepu
enum ShopSubState {
    SHOP_MAIN,
    SHOP_BALL_INVENTORY
};

struct ShopButton {
    sf::RectangleShape shape;
    std::string nazwa;
    std::string opis;
    int cena;
    bool isHovered = false;
};

class ShopMenu : public MenuScreen {
private:
    ShopSubState currentSubState = SHOP_MAIN;

    sf::RectangleShape bg;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;

    std::vector<ShopButton> cueButtons;
    std::vector<ShopButton> ballButtons;

    sf::RectangleShape btnNextShape;
    sf::Text btnNextText;

    sf::Font font;
    sf::Text titleText;

    // Srodkowe elementy
    sf::RectangleShape currentCueDisplay;
    sf::ConvexShape currentTriangleDisplay; // Ikona trójkąta
    sf::Text inventoryText;

    // Plywajace okno
    sf::RectangleShape tooltipBg;
    sf::Text tooltipName;
    sf::Text tooltipDesc;
    sf::Text tooltipPrice;
    bool showTooltip = false;

    // Szczegoly bil
    sf::RectangleShape darkOverlay;
    std::vector<sf::CircleShape> inventoryBalls;
    sf::Text closePromptText;

    // Losowanie sklepu
    std::vector<Upgrade> wylosowaneKije;
    std::vector<Upgrade> wylosowaneBile;

public:
    ShopMenu(std::pair<int,int> res);

    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
};
#endif // SHOPMENU_H
