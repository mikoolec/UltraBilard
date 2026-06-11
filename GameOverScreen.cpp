#include "GameOverScreen.h"
#include <iostream>

void GameOverScreen::loadAndSetup(sf::Texture& tex, sf::Sprite& spr, std::string path) {
    if(!tex.loadFromFile(path)) std::cout<<"brak "<<path<<std::endl;
    tex.setSmooth(false); spr.setTexture(tex);
}

GameOverScreen::GameOverScreen(std::pair<int,int> res) {
    bgFilter.setSize(sf::Vector2f(res.first,res.second));
    bgFilter.setFillColor(sf::Color(0,0,0,200));

    loadAndSetup(texPanel, sprPanel, "assets//GO_tlo.png");
    loadAndSetup(texTitle, sprTitle, "assets//GO_GO.png");
    loadAndSetup(texLine, sprLine, "assets//GO_prosta.png");
    loadAndSetup(texCoinIcon, sprCoinIcon, "assets//monetka.png");

    loadAndSetup(texLabelScore, sprLabelScore, "assets//GO_zdobyte_punkty.png");
    loadAndSetup(texLabelCoins, sprLabelCoins, "assets//GO_zebrane_monety.png");
    loadAndSetup(texLabelRounds, sprLabelRounds, "assets//GO_rundy.png");
    loadAndSetup(texLabelBiles, sprLabelBiles, "assets//GO_wbite_bile.png");
    loadAndSetup(texLabelShots, sprLabelShots, "assets//GO_ilosc_strzalow.png");

    texBtnRetryNorm.loadFromFile("assets//GO_new_run_normal.png"); texBtnRetryNorm.setSmooth(false);
    texBtnRetryHov.loadFromFile("assets//GO_new_run_hover.png"); texBtnRetryHov.setSmooth(false);
    texBtnMenuNorm.loadFromFile("assets//GO_main_menu_normal.png"); texBtnMenuNorm.setSmooth(false);
    texBtnMenuHov.loadFromFile("assets//GO_main_menu_hover.png"); texBtnMenuHov.setSmooth(false);

    sf::Vector2f center(res.first/2.0f,res.second/2.0f);

    sf::FloatRect pb = sprPanel.getLocalBounds(); sprPanel.setOrigin(pb.width/2.0f, pb.height/2.0f); sprPanel.setPosition(center);
    sf::FloatRect tb = sprTitle.getLocalBounds(); sprTitle.setOrigin(tb.width/2.0f, tb.height/2.0f); sprTitle.setPosition(center.x, center.y - 130); sprTitle.setScale(1.8f,1.8f);
    sf::FloatRect lb = sprLine.getLocalBounds(); sprLine.setOrigin(lb.width/2.0f, lb.height/2.0f); sprLine.setPosition(center.x, center.y - 110);

    float statsLeftX = center.x - 130; float statsValueX = center.x + 40;
    sprLabelScore.setPosition(statsLeftX, center.y - 80);
    sprLabelCoins.setPosition(statsLeftX, center.y - 50);
    sprLabelRounds.setPosition(statsLeftX, center.y - 20);
    sprLabelBiles.setPosition(statsLeftX, center.y + 10);
    sprLabelShots.setPosition(statsLeftX, center.y + 40);

    sprBtnRetry.setTexture(texBtnRetryNorm); sf::FloatRect rb = sprBtnRetry.getLocalBounds();
    sprBtnRetry.setOrigin(rb.width/2.0f, rb.height/2.0f); sprBtnRetry.setPosition(center.x, center.y + 90); sprBtnRetry.setScale(0.6f,0.6f);

    sprBtnMenu.setTexture(texBtnMenuNorm); sf::FloatRect mb = sprBtnMenu.getLocalBounds();
    sprBtnMenu.setOrigin(mb.width/2.0f, mb.height/2.0f); sprBtnMenu.setPosition(center.x, center.y + 130); sprBtnMenu.setScale(0.6f,0.6f);

    if (font.loadFromFile("assets//PublicPixel.ttf")) {
        valScore.setFont(font); valScore.setCharacterSize(16); valScore.setFillColor(sf::Color::White);
        valCoins.setFont(font); valCoins.setCharacterSize(16); valCoins.setFillColor(sf::Color::White);
        valRounds.setFont(font); valRounds.setCharacterSize(16); valRounds.setFillColor(sf::Color::White);
        valBiles.setFont(font); valBiles.setCharacterSize(16); valBiles.setFillColor(sf::Color::White);
        valShots.setFont(font); valShots.setCharacterSize(16); valShots.setFillColor(sf::Color::White);

        valScore.setPosition(statsValueX, sprLabelScore.getPosition().y - 3);
        valCoins.setPosition(statsValueX, sprLabelCoins.getPosition().y - 3);
        valRounds.setPosition(statsValueX, sprLabelRounds.getPosition().y - 3);
        valBiles.setPosition(statsValueX, sprLabelBiles.getPosition().y - 3);
        valShots.setPosition(statsValueX, sprLabelShots.getPosition().y - 3);
    }
}

void GameOverScreen::updateStats(const GameStats& stats) {
    valScore.setString(std::to_string(stats.punktyGlobalnie));
    valCoins.setString(std::to_string(stats.monetyGlobalnie));
    valRounds.setString(std::to_string(stats.rundy));
    valBiles.setString(std::to_string(stats.wbiteBileGlobalnie));
    valShots.setString(std::to_string(stats.strzalyGlobalnie));

    float rightEdgeX = sprPanel.getPosition().x + 120.0f;
    auto alignTextRight = [&](sf::Text& text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left+bounds.width,0);
        text.setPosition(rightEdgeX,text.getPosition().y);
    };
    alignTextRight(valScore); alignTextRight(valCoins); alignTextRight(valRounds); alignTextRight(valBiles); alignTextRight(valShots);
    sprCoinIcon.setPosition(rightEdgeX+10.0f,valCoins.getPosition().y+2.0f);
}

void GameOverScreen::updateHover(sf::Vector2f mousePos) {
    if (sprBtnRetry.getGlobalBounds().contains(mousePos)) sprBtnRetry.setTexture(texBtnRetryHov);
    else sprBtnRetry.setTexture(texBtnRetryNorm);

    if (sprBtnMenu.getGlobalBounds().contains(mousePos)) sprBtnMenu.setTexture(texBtnMenuHov);
    else sprBtnMenu.setTexture(texBtnMenuNorm);
}

int GameOverScreen::handleClick(sf::Vector2f mousePos) {
    if (sprBtnRetry.getGlobalBounds().contains(mousePos)) return 4; // Retry
    if (sprBtnMenu.getGlobalBounds().contains(mousePos)) return 5; // To Menu
    return 0;
}

void GameOverScreen::draw(sf::RenderTexture& target) {
    target.draw(bgFilter); target.draw(sprPanel); target.draw(sprTitle); target.draw(sprLine);
    target.draw(sprLabelScore); target.draw(sprLabelCoins); target.draw(sprCoinIcon); target.draw(sprLabelRounds); target.draw(sprLabelBiles); target.draw(sprLabelShots);
    target.draw(valScore); target.draw(valCoins); target.draw(valRounds); target.draw(valBiles); target.draw(valShots);
    target.draw(sprBtnRetry); target.draw(sprBtnMenu);
}