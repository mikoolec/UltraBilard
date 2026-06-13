#include "LevelManager.h"
#include "Globals.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

LevelManager::LevelManager() {
    celPunktow = 1;
    obecnyBoss = BossType::None;
    zmniejszoneTarcieAktywne = false;
    tarcieStoluGlobal = 1.0f;
}

float LevelManager::losujFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

bool LevelManager::isBossRound(int runda) {
    return runda > 0 && runda % 5 == 0;
}

BossType LevelManager::bossForRound(int runda) {
    if (!isBossRound(runda)) return BossType::None;

    int bossIndex = ((runda / 5) - 1) % 3;
    if (bossIndex == 0) return BossType::Mud;
    if (bossIndex == 1) return BossType::Ice;
    return BossType::BlockedHoles;
}

void LevelManager::ustawCelPunktow() {
    //celPunktow = static_cast<int>(2 * std::pow(1.5, g_Stats.rundy - 1));
    celPunktow = 10 + (g_Stats.rundy * 3);
}

bool LevelManager::czyDziuraZablokowana(int indeksDziury) const {
    return std::find(zablokowaneDziury.begin(), zablokowaneDziury.end(), indeksDziury) != zablokowaneDziury.end();
}

void LevelManager::losujPlamyBlota() {
    plamyBlota.clear();
    int liczbaPlam = 3 + (std::rand() % 3);
    float promienBlota = 28.f; // Założona rozdzielczość wirtualna 640x360
    float minX = 20.f + promienBlota;
    float maxX = 620.f - promienBlota;
    float minY = 24.f + promienBlota;
    float maxY = 334.f - promienBlota;

    for (int i = 0; i < liczbaPlam; i++) {
        sf::CircleShape plama(promienBlota);
        plama.setOrigin(promienBlota, promienBlota);
        plama.setPosition(losujFloat(minX, maxX), losujFloat(minY, maxY));
        plama.setFillColor(sf::Color(92, 62, 31, 140));
        plama.setOutlineColor(sf::Color(55, 39, 21, 190));
        plama.setOutlineThickness(1.f);
        plamyBlota.push_back(plama);
    }
}

void LevelManager::losujZablokowaneDziury() {
    zablokowaneDziury.clear();
    int iloscZablokowanych = 1 + ((g_Stats.rundy + 14) / 15);
    iloscZablokowanych = std::min(5, iloscZablokowanych);

    std::vector<int> dostepneDziury = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < iloscZablokowanych; i++) {
        int indeks = std::rand() % static_cast<int>(dostepneDziury.size());
        zablokowaneDziury.push_back(dostepneDziury[indeks]);
        dostepneDziury.erase(dostepneDziury.begin() + indeks);
    }
}

void LevelManager::przygotujRunde() {
    ustawCelPunktow();
    plamyBlota.clear();
    zablokowaneDziury.clear();
    zmniejszoneTarcieAktywne = false;
    tarcieStoluGlobal = 1.f;
    obecnyBoss = bossForRound(g_Stats.rundy);

    if (obecnyBoss == BossType::Mud) { losujPlamyBlota(); }
    else if (obecnyBoss == BossType::Ice) { zmniejszoneTarcieAktywne = true; tarcieStoluGlobal = tarcieLodu; }
    else if (obecnyBoss == BossType::BlockedHoles) { losujZablokowaneDziury(); }

    std::cout << "Runda " << g_Stats.rundy << ", cel punktow: " << celPunktow << std::endl;
}