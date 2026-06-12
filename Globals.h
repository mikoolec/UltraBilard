#ifndef GLOBALS_H
#define GLOBALS_H
#pragma once

enum GameState {
    MENU,
    PLAYING,
    SHOP,
    GAME_OVER
};

struct GameStats {
    int punktyTejRundy = 0;
    int punktyGlobalnie = 0;
    int monety = 100;                                                   // do wyzerowania!!!!!!!!
    int monetyGlobalnie = 0;
    int rundy = 1;
    int wbiteBileGlobalnie = 0;
    int strzalyGlobalnie = 0;

    bool kupUlepszenie(int cena) {
        if (monety >= cena) {
            monety -= cena;
            return true; // Zakup udany
        }
        return false; // Brak środków
    }
};

// Deklaracja, że zmienna g_Stats istnieje
extern GameStats g_Stats;
#endif // GLOBALS_H
