#ifndef GLOBALS_H
#define GLOBALS_H
#pragma once
#include<vector>
#include<map>

enum GameState {
    MENU,
    PLAYING,
    SHOP,
    GAME_OVER
};

struct GameStats {
    int punktyTejRundy = 0;
    int punktyGlobalnie = 0;
    int monety = 1000;                                                   // do wyzerowania!!!!!!!!
    int monetyGlobalnie = 0;
    int rundy = 1;
    int wbiteBileGlobalnie = 0;
    int strzalyGlobalnie = 0;
    std::vector<int> posiadaneUpgradeID;
    std::map<int, std::vector<int>> ulepszeniaBil;

    bool kupUlepszenie(int cena, int id) {
        if (monety >= cena) {
            monety -= cena;
            posiadaneUpgradeID.push_back(id); // dodaje item do eq
            return true;
        }
        return false;
    }

    void dodajUpgrade(int id)
    {
        posiadaneUpgradeID.push_back(id);
    }

    bool czyPosiada(int id)
    {
        if (id < 200)
        {
            for(int posiadaneID : posiadaneUpgradeID)
            {
                if(posiadaneID == id) return true;
            }
        }
        return false;
    }
};

// Deklaracja, że zmienna g_Stats istnieje
extern GameStats g_Stats;
#endif // GLOBALS_H
