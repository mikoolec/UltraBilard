#ifndef GLOBALS_H
#define GLOBALS_H
#pragma once
#include<vector>
#include<map>
#include <fstream>
#include<iostream>

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

    void ResetujGre()
    {
        punktyTejRundy = 0;
        punktyGlobalnie = 0;
        monety = 1000; // startowa ilosc do testow
        monetyGlobalnie = 0;
        rundy = 1;
        wbiteBileGlobalnie = 0;
        strzalyGlobalnie = 0;

        // clear eq
        posiadaneUpgradeID.clear();
        ulepszeniaBil.clear();
    }

    inline void ZapiszGre() {
        std::ofstream plik("save.txt");
        if (plik.is_open()) {
            plik << monety << "\n";
            plik << punktyGlobalnie << "\n";

            // 1. Zapis ekwipunku Kijów (najpierw ilość, potem same ID)
            plik << posiadaneUpgradeID.size() << "\n";
            for (int id : posiadaneUpgradeID) plik << id << "\n";

            // 2. Zapis ulepszeń dla 15 Bil
            for (int i = 0; i < 15; i++) {
                plik << ulepszeniaBil[i].size() << "\n";
                for (int id : ulepszeniaBil[i]) {
                    plik << id << "\n";
                }
            }

            plik.close();
            std::cout << "ZAPISANO STAN GRY!" << std::endl;
        }
    }

    inline void WczytajGre() {
        std::ifstream plik("save.txt");
        if (plik.is_open()) {
            plik >> monety;
            plik >> punktyGlobalnie;

            // 1. Wczytywanie Kijów
            int iloscKijow;
            if (plik >> iloscKijow) {
                posiadaneUpgradeID.clear();
                for (int i = 0; i < iloscKijow; i++) {
                    int id; plik >> id;
                    posiadaneUpgradeID.push_back(id);
                }
            }

            // 2. Wczytywanie Bil
            for (int i = 0; i < 15; i++) {
                int iloscUlepszenBili;
                if (plik >> iloscUlepszenBili) {
                    ulepszeniaBil[i].clear();
                    for (int j = 0; j < iloscUlepszenBili; j++) {
                        int id; plik >> id;
                        ulepszeniaBil[i].push_back(id);
                    }
                }
            }

            plik.close();
            std::cout << "WCZYTANO STAN GRY!" << std::endl;
        } else {
            std::cout << "Brak pliku save.txt (To normalne przy pierwszym uruchomieniu)." << std::endl;
        }
    }
};

// Deklaracja, że zmienna g_Stats istnieje
extern GameStats g_Stats;
#endif // GLOBALS_H
