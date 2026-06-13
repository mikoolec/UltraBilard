#ifndef MAINMENU_H
#define MAINMENU_H
#pragma once
#include "MenuScreen.h"
#include <string>

class MainMenu : public MenuScreen {
private:
    sf::Texture textNewRunNormal, textNewRunHover, textContinueNormal, textContinueHover, textSettingsNormal, textSettingsHover, textQuitNormal, textQuitHover;
    sf::Sprite btnNewRun, btnContinue, btnSettings, btnQuit;
    sf::Font font;
    sf::Text titleText;

    void setupButton(sf::Sprite& btn, sf::Texture& textNorm, sf::Texture& textHov, std::string pathNorm, std::string pathHov, float x, float y);

public:
    MainMenu(std::pair<int,int> res);
    void updateHover(sf::Vector2f mousePos) override;
    int handleClick(sf::Vector2f mousePos) override;
    void draw(sf::RenderTexture& target) override;
};

#endif // MAINMENU_H
