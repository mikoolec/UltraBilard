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
    int monety = 0;
    int monetyGlobalnie = 0;
    int rundy = 1;
    int wbiteBileGlobalnie = 0;
    int strzalyGlobalnie = 0;
};

// Deklaracja, że zmienna g_Stats istnieje
extern GameStats g_Stats;
#endif // GLOBALS_H
