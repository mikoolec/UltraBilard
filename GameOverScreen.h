#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H
#pragma once
#include "MenuScreen.h"
#include "Globals.h"
#include <string>

class GameOverScreen : public MenuScreen {
private:
    sf::RectangleShape bgFilter;
    sf::Texture texPanel,texTitle,texLine;
    sf::Texture texLabelScore, texLabelRounds, texLabelBiles, texLabelShots;
    sf::Texture texBtnRetryNorm, texBtnRetryHov, texBtnMenuNorm, texBtnMenuHov;

    sf::Sprite sprPanel,sprTitle,sprLine;
    sf::Sprite sprLabelScore, sprLabelRounds, sprLabelBiles, sprLabelShots;
    sf::Sprite sprBtnRetry, sprBtnMenu;

    sf::Font font;
    sf::Text valScore, valRounds, valBiles, valShots;

    void loadAndSetup(sf::Texture& tex, sf::Sprite& spr, std::string path);
public:
    GameOverScreen(std::pair<int,int> res);
    void updateStats(const GameStats& stats);
    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
};

#endif // GAMEOVERSCREEN_H
