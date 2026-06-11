#ifndef SHOPMENU_H
#define SHOPMENU_H
#pragma once
#include "MenuScreen.h"
#include <vector>
#include <string>

// Tymczasowa struktura trzymająca dane przedmiotu w sklepie
struct ShopButton {
    sf::RectangleShape shape;
    std::string nazwa;
    std::string opis;
    int cena;
    bool isHovered = false;
};

class ShopMenu : public MenuScreen {
private:
    sf::RectangleShape bg;
    sf::RectangleShape leftPanel;
    sf::RectangleShape rightPanel;

    // Listy kiji oraz bil
    std::vector<ShopButton> cueButtons;
    std::vector<ShopButton> ballButtons;

    // Przycisk Dalej
    sf::RectangleShape btnNextShape;
    sf::Text btnNextText;

    // Tekst środek
    sf::Font font;
    sf::Text titleText;
    sf::Text tooltipName;
    sf::Text tooltipDesc;
    sf::Text tooltipPrice;

    // Zmienna trzymająca wskaźnik na aktualnie najechaną myszką kartę
    ShopButton* hoveredItem = nullptr;

public:
    ShopMenu(std::pair<int,int> res);

    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
};

#endif // SHOPMENU_H
