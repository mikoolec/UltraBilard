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
    sf::Sprite sprite;
    std::string nazwa;
    std::string opis;
    int cena;
    bool isHovered = false;
    bool kupiony = false;
    int id;
};

class ShopMenu : public MenuScreen {
private:
    ShopSubState currentSubState = SHOP_MAIN;

    sf::RectangleShape bg;
    sf::Texture texTloCegly;
    sf::Sprite sprTloCegly;

    sf::Texture texDrewnoSrodek;
    sf::Sprite sprDrewnoSrodek;

    sf::Texture texBoxKijNorm, texBoxKijHov;
    sf::Texture texBoxBilaNorm, texBoxBilaHov;

    sf::Texture texKijSrodek;
    sf::Sprite sprKijSrodek;

    sf::Texture texTrojkatSrodek;
    sf::Sprite sprTrojkatSrodek;

    sf::Texture texBtnRefreshNorm, texBtnRefreshHov;
    sf::Sprite sprBtnRefresh;

    sf::Texture texBtnNextNorm, texBtnNextHov;
    sf::Sprite sprBtnNext;

    std::vector<ShopButton> cueButtons;
    std::vector<ShopButton> ballButtons;

    sf::Text btnNextText;

    sf::Font font;
    sf::Text titleText;

    // Srodkowe elementy
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

    ShopButton* hoveredItem = nullptr;

    // Refresh button
    int kosztRefresha = 10;

    ShopButton* pendingBallUpgrade = nullptr;

public:
    ShopMenu(std::pair<int,int> res);

    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
    void OdswiezPrzedmioty();
};
#endif // SHOPMENU_H
