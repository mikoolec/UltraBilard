#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

enum class BossType
{
    None,
    Mud,
    Ice,
    BlockedHoles
};

class LevelManager {
public:
    int celPunktow;
    BossType obecnyBoss;
    std::vector<sf::CircleShape> plamyBlota;
    std::vector<int> zablokowaneDziury;
    bool zmniejszoneTarcieAktywne;
    float tarcieStoluGlobal;

    const float tarcieLodu = 0.35f;
    const float mnoznikTarciaBlota = 3.5f;

    LevelManager();
    
    void przygotujRunde();
    bool czyDziuraZablokowana(int indeksDziury) const;

private:
    float losujFloat(float min, float max);
    bool isBossRound(int runda);
    BossType bossForRound(int runda);
    void ustawCelPunktow();
    void losujPlamyBlota();
    void losujZablokowaneDziury();
};

#endif