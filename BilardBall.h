#ifndef BILARDBALL_H
#define BILARDBALL_H
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "BilardHole.h"

class BilardBall : public sf::CircleShape, public GameObject {
public:
    // Zmienne do ulepszeń
    float tarcieBaza, tarciescianBaza, punktyBaza, mnoznikBaza, wielkoscBaza;
    int hitpunktyBaza, hitscianaBaza, monetyBaza, hitmonetyBaza;

    // Zmienne w grze
    float tarcie, tarciescian, radiusMultipl, mnoznikPunktowy;
    int wartoscPunktowa, punktyNaUderzeniuKuli, punktyNaUderzeniuSciany;
    float aktywneTarcieStolu = 1.0f;

    // Zmienne stanu
    bool Held = false;
    bool Put = false;
    int identifier;
    std::vector<int> saveBounces;
    sf::Vector2f velocity;
    int rotation;
    sf::IntRect bounds;

    sf::Sound wallhitSFX;
    sf::Sound ballhitSFX;

    // Konstruktor
    BilardBall(float radius, const sf::Vector2f& position, int id);

    // Metody wymuszone przez GameObject (Polimorfizm)
    void update(const sf::Time& elapsed) override;
    void draw(sf::RenderTexture& target) override;

    // Metody własne Bili
    void resetStats();
    void rob_tarcie(float sila, bool sciana);
    void ballPut();
    void setVelocity(sf::Vector2f vel);
    bool stationary();
    void cleanBounces();

    // Nowe, polimorficzne kolizje
    void kolizjeKul(const sf::Time& elapsed, std::vector<std::unique_ptr<GameObject>>& entities, float tarcieScianGlobal, float tarcieStoluGlobal);
    void kolizjeScian(const sf::Time& elapsed, float tarcieScianGlobal, float tarcieStoluGlobal);
    void kolizjeDziur(const sf::Time& elapsed, std::vector<std::unique_ptr<GameObject>>& entities, float tarcieScianGlobal, float tarcieStoluGlobal);
};

#endif // BILARDBALL_H
