#include "MainMenu.h"
#include <iostream>
#include<fstream>

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

    setupButton(btnNewRun, textNewRunNormal, textNewRunHover, "assets//przycisk_start_normal.png","assets//przycisk_start_hover.png", res.first/2.0f, 190);
    setupButton(btnContinue, textContinueNormal, textContinueHover, "assets//SHOP_continue_normal.png","assets//SHOP_continue_hover.png", res.first/2.0f, 250);
    setupButton(btnQuit, textQuitNormal, textQuitHover, "assets//przycisk_quit_normal.png","assets//przycisk_quit_hover.png", res.first/2.0f, 310);

    // Sprawdzanie czy plik istnieje
    std::ifstream plik("save.txt");
    if (plik.is_open()) {
        moznaKontynuowac = true;
        plik.close();
    } else {
        moznaKontynuowac = false;
        btnContinue.setColor(sf::Color(100, 100, 100, 150)); // Przyciemniamy guzik na szaro!
    }
}

void MainMenu::updateHover(sf::Vector2f mousePos) {
    if(btnNewRun.getGlobalBounds().contains(mousePos)) { btnNewRun.setTexture(textNewRunHover); btnNewRun.setScale(1.1f,1.1f); }
    else { btnNewRun.setTexture(textNewRunNormal); btnNewRun.setScale(1.0f,1.0f); }

    if (moznaKontynuowac) {
        if(btnContinue.getGlobalBounds().contains(mousePos)) { btnContinue.setTexture(textContinueHover); btnContinue.setScale(1.1f,1.1f); }
        else { btnContinue.setTexture(textContinueNormal); btnContinue.setScale(1.0f,1.0f); }
    }

    if(btnQuit.getGlobalBounds().contains(mousePos)) { btnQuit.setTexture(textQuitHover); btnQuit.setScale(1.1f,1.1f); }
    else { btnQuit.setTexture(textQuitNormal); btnQuit.setScale(1.0f,1.0f); }
}

int MainMenu::handleClick(sf::Vector2f mousePos) {
    if(btnNewRun.getGlobalBounds().contains(mousePos)) return 1; // Nowa gra
    if(btnContinue.getGlobalBounds().contains(mousePos) && moznaKontynuowac) return 7; // Kontynuuj
    if(btnQuit.getGlobalBounds().contains(mousePos)) return 3; // Wyjście z gry
    return 0;
}

void MainMenu::draw(sf::RenderTexture& target) {
    target.draw(titleText);
    target.draw(btnNewRun);
    target.draw(btnContinue);
    target.draw(btnQuit);
}