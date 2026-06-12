#ifndef HUD_H
#define HUD_H
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
private:
    sf::Font hudFont;
    void rysujBelke(sf::RenderTexture& target, sf::Vector2f pozycja, const std::string& tekst, sf::Color akcent);

public:
    HUD();
    void draw(sf::RenderTexture& target, int aktualneMaxStrzaly, int strzaly, int celPunktow, const std::string& etykietaRundy);
};

#endif
