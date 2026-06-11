#include "MainMenu.h"
#include <iostream>

void MainMenu::setupButton(sf::Sprite& btn, sf::Texture& textNorm, sf::Texture& textHov, std::string pathNorm, std::string pathHov, float x, float y) {
    if(!textNorm.loadFromFile(pathNorm) || !textHov.loadFromFile(pathHov)) std::cout << "Brak grafik menu\n";
    btn.setTexture(textNorm);
    sf::FloatRect bounds = btn.getLocalBounds();
    btn.setOrigin(bounds.width/2.0f, bounds.height/2.0f);
    btn.setPosition(x,y);
}

MainMenu::MainMenu(std::pair<int,int> res) {
    if(font.loadFromFile("assets//PublicPixel.ttf")) {
        titleText.setFont(font); titleText.setString("UltraBilard");
        titleText.setCharacterSize(50); titleText.setFillColor(sf::Color::White);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width /2.0f, tb.top + tb.height /2.0f);
        titleText.setPosition(res.first/2.0f, 60);
    }
    setupButton(btnStart, textStartNormal, textStartHover, "assets//przycisk_start_normal.png","assets//przycisk_start_hover.png", res.first/2.0f, 160);
    setupButton(btnSettings, textSettingsNormal, textSettingsHover, "assets//przycisk_settings_normal.png","assets//przycisk_settings_hover.png", res.first/2.0f, 230);
    setupButton(btnQuit, textQuitNormal, textQuitHover, "assets//przycisk_quit_normal.png","assets//przycisk_quit_hover.png", res.first/2.0f, 300);
}

void MainMenu::updateHover(sf::Vector2f mousePos) {
    if(btnStart.getGlobalBounds().contains(mousePos)) { btnStart.setTexture(textStartHover); btnStart.setScale(1.1f,1.1f); }
    else { btnStart.setTexture(textStartNormal); btnStart.setScale(1.0f,1.0f); }

    if(btnSettings.getGlobalBounds().contains(mousePos)) { btnSettings.setTexture(textSettingsHover); btnSettings.setScale(1.1f,1.1f); }
    else { btnSettings.setTexture(textSettingsNormal); btnSettings.setScale(1.0f,1.0f); }

    if(btnQuit.getGlobalBounds().contains(mousePos)) { btnQuit.setTexture(textQuitHover); btnQuit.setScale(1.1f,1.1f); }
    else { btnQuit.setTexture(textQuitNormal); btnQuit.setScale(1.0f,1.0f); }
}

int MainMenu::handleClick(sf::Vector2f mousePos) {
    if(btnStart.getGlobalBounds().contains(mousePos)) return 1; // Play
    if(btnSettings.getGlobalBounds().contains(mousePos)) return 2; // Settings
    if(btnQuit.getGlobalBounds().contains(mousePos)) return 3; // Quit
    return 0;
}

void MainMenu::draw(sf::RenderTexture& target) {
    target.draw(titleText); target.draw(btnStart); target.draw(btnSettings); target.draw(btnQuit);
}