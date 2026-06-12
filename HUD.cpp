#include "HUD.h"
#include "Globals.h" // by mieć dostęp do g_Stats
#include <iostream>

HUD::HUD() {
    if (!hudFont.loadFromFile("assets//PublicPixel.ttf")) {
        std::cout << "Blad: Nie znaleziono pliku PublicPixel.ttf w klasie HUD!" << std::endl;
    }
}

void HUD::rysujBelke(sf::RenderTexture& target, sf::Vector2f pozycja, const std::string& tekst, sf::Color akcent) {
    sf::RectangleShape tloBelki(sf::Vector2f(136.f, 18.f));
    tloBelki.setPosition(pozycja);
    tloBelki.setFillColor(sf::Color(14, 18, 22, 220));
    tloBelki.setOutlineColor(sf::Color(215, 219, 212, 210));
    tloBelki.setOutlineThickness(1.f);

    sf::RectangleShape pasekAkcentu(sf::Vector2f(4.f, 14.f));
    pasekAkcentu.setPosition(pozycja.x + 3.f, pozycja.y + 2.f);
    pasekAkcentu.setFillColor(akcent);

    sf::Text napis;
    napis.setFont(hudFont);
    napis.setCharacterSize(8);
    napis.setFillColor(sf::Color::White);
    napis.setString(tekst);
    napis.setPosition(pozycja.x + 12.f, pozycja.y + 5.f);

    target.draw(tloBelki);
    target.draw(pasekAkcentu);
    target.draw(napis);
}

void HUD::draw(sf::RenderTexture& target, int aktualneMaxStrzaly, int strzaly, int celPunktow, const std::string& etykietaRundy) {
    int pozostaleStrzaly = std::max(0, aktualneMaxStrzaly - strzaly);
    rysujBelke(target, sf::Vector2f(8.f, 5.f), etykietaRundy, sf::Color(105, 180, 255));
    rysujBelke(target, sf::Vector2f(160.f, 5.f), "STRZALY: " + std::to_string(pozostaleStrzaly), sf::Color(255, 220, 105));
    rysujBelke(target, sf::Vector2f(312.f, 5.f), "PKT: " + std::to_string(g_Stats.punktyTejRundy) + "/" + std::to_string(celPunktow), sf::Color(120, 230, 155));
    rysujBelke(target, sf::Vector2f(464.f, 5.f), "MONETY: " + std::to_string(g_Stats.monety), sf::Color(255, 190, 70));
}
