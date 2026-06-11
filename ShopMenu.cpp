#include "ShopMenu.h"
#include <iostream>

ShopMenu::ShopMenu(std::pair<int,int> res) {
    bg.setSize(sf::Vector2f(res.first,res.second));
    bg.setFillColor(sf::Color::White);

    if(font.loadFromFile("assets//PublicPixel.ttf")) {
        titleText.setFont(font); titleText.setString("sklep test");
        titleText.setCharacterSize(30); titleText.setFillColor(sf::Color::Black);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width /2.0f, tb.top+tb.height/2.0f);
        titleText.setPosition(res.first/2.0f,80);
    }

    if (!texBtnNextNorm.loadFromFile("assets//SHOP_continue_normal.png") || !texBtnNextHov.loadFromFile("assets//SHOP_continue_hover.png")) {
        std::cout << "brak przycisku do shop" << std::endl;
    }
    texBtnNextNorm.setSmooth(false); texBtnNextHov.setSmooth(false);

    btnNext.setTexture(texBtnNextNorm);
    sf::FloatRect bounds = btnNext.getLocalBounds();
    btnNext.setOrigin(bounds.width/2.0f,bounds.height/2.0f);
    btnNext.setPosition(res.first/2.0f,res.second/2.0f+50);
}

void ShopMenu::updateHover(sf::Vector2f mousePos) {
    if(btnNext.getGlobalBounds().contains(mousePos)) btnNext.setTexture(texBtnNextHov);
    else btnNext.setTexture(texBtnNextNorm);
}

int ShopMenu::handleClick(sf::Vector2f mousePos) {
    if (btnNext.getGlobalBounds().contains(mousePos)) return 6; // Next Round
    return 0;
}

void ShopMenu::draw(sf::RenderTexture& target) {
    target.draw(bg); target.draw(titleText); target.draw(btnNext);
}