#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include<ctime>
#include <cmath>
#include <cstdlib>
#include <string>

// nagłówki
#include "Globals.h"
#include "AudioManager.h"
#include "MathUtils.h"
#include "GameObject.h"
#include "BilardBall.h"
#include "BilardHole.h"
#include "MenuScreen.h"
#include "MainMenu.h"
#include "ShopMenu.h"
#include "GameOverScreen.h"
#include "HUD.h"
#include "LevelManager.h"

using namespace std;

// Definicja globalnej zmiennej z Globals.h
GameStats g_Stats;

void RysujLinie(sf::RenderTexture& target, sf::Vector2f punktA, sf::Vector2f punktB, float grubosc, sf::Color kolor)
{
    // 1. Obliczamy wektor różnicy między punktami
    sf::Vector2f kierunek = punktB - punktA;

    // 2. Liczymy długość linii (z twierdzenia Pitagorasa)
    float dlugosc = sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);

    // 3. Tworzymy prostokąt, który posłuży za linię
    sf::RectangleShape linia(sf::Vector2f(dlugosc, grubosc));
    linia.setFillColor(kolor);

    // 4. KLUCZOWE: Ustawiamy punkt obrotu na środek lewej krawędzi,
    // dzięki czemu linia będzie idealnie wycentrowana wokół punktu A
    linia.setOrigin(0, grubosc / 2.f);

    // 5. Pozycjonujemy linię w punkcie startowym
    linia.setPosition(punktA);

    // 6. Obliczamy kąt w radianach i zamieniamy go na stopnie dla SFML
    float kat = std::atan2(kierunek.y, kierunek.x) * 180.f / 3.14159265f;
    linia.setRotation(kat);

    // 7. Rysujemy linię na naszym wirtualnym ekranie
    target.draw(linia);
}

void ZaladujTekstureTla(sf::Texture& tlo_stolu)
{
    if (!tlo_stolu.loadFromFile("assets//stol_tlo.png")) {
        std::cout << "Blad: Nie znaleziono pliku stol_tlo.png!" << std::endl;
    }
    tlo_stolu.setSmooth(false);
}
void ZaladujTekstureScian(sf::Texture& sciany_stolu)
{
    if (!sciany_stolu.loadFromFile("assets//stol_sciany.png")) {
        std::cout << "Blad: Nie znaleziono pliku stol_sciany.png!" << std::endl;
    }
    sciany_stolu.setSmooth(false);
}

// funkcja resetujące korzystające z głównego kontenera
void resetBoard(std::vector<std::unique_ptr<GameObject>>& entities, const std::vector<float>& MiejscaX, const std::vector<float>& MiejscaY, int& shots) {
    std::cout << "debug - Start resetBoard..." << std::endl;
    for (auto& obj : entities) {
        if (auto* bal = dynamic_cast<BilardBall*>(obj.get())) {
            bal->Put = false;
            if (bal->identifier >= 0 && bal->identifier < MiejscaX.size()) {
                bal->setPosition(MiejscaX[bal->identifier], MiejscaY[bal->identifier]);
            }
            bal->setVelocity(sf::Vector2f(0, 0));
            bal->resetStats();
        }
    }
    shots = 0;
    g_Stats.punktyTejRundy = 0;
    std::cout << "debug - Koniec resetBoard Pomyslnie ulozono bile." << std::endl;
}

void mult( std::vector<float> &vec, float num )
{
    for( auto &v : vec )
    {
        v *= num;
    }
}

void resetCalejRozgrywki(std::vector<std::unique_ptr<GameObject>>& entities, const std::vector<float>& MiejscaX, const std::vector<float>& MiejscaY, int& shots)
{
    resetBoard(entities, MiejscaX, MiejscaY, shots);
    g_Stats.punktyGlobalnie = 0;
    g_Stats.monety = 1000; // test
    g_Stats.monetyGlobalnie = 0;
    g_Stats.rundy = 1;
    g_Stats.wbiteBileGlobalnie = 0;
    g_Stats.strzalyGlobalnie = 0;

    // clear eq
    g_Stats.posiadaneUpgradeID.clear();
    g_Stats.ulepszeniaBil.clear();

    std::cout<<"debug - resetujemy runa i czyscimy ekwipunek"<<std::endl;
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(640, 360), "UltraBilard");

    // Uzywac tej zmiennej w ustawieniach
    // Wypróbowane wartości: 640,360 ; 320,180 ; 480,270 ; 256,144 ; 128,72 ; 1280,720
    pair <int,int> rozdzielczosc = make_pair(640,360);
    // Przygotowanie stylizowanego ekranu:
    sf::RenderTexture virtualScreen;
    virtualScreen.create(rozdzielczosc.first,rozdzielczosc.second);
    virtualScreen.setSmooth(false);


    // Importowanie tła:
    sf::Texture tlo_stolu;
    ZaladujTekstureTla(tlo_stolu);
    sf::Sprite tlo;
    tlo.setTexture(tlo_stolu);

    // Importowanie scian:
    sf::Texture sciany_stol;
    ZaladujTekstureScian(sciany_stol);
    sf::Sprite sciany;
    sciany.setTexture(sciany_stol);

    // Efekt lodu na istniejacym tle stolu
    sf::RectangleShape lodVisual(sf::Vector2f(rozdzielczosc.first, rozdzielczosc.second));
    lodVisual.setFillColor(sf::Color(70, 165, 255, 115));

    // Importowanie kija
    sf::Texture texKij;
    if (!texKij.loadFromFile("assets//kij.png")) {
        std::cout << "Blad: Nie znaleziono pliku kij.png!" << std::endl;
    }
    texKij.setSmooth(false);
    sf::Sprite sprKij(texKij);
    sprKij.setOrigin(0, texKij.getSize().y / 2.0f);

    // Dzwieki
    g_Audio.init();

    // Skalowanie tla na cały ekran:
    float scaleXtlo = (float)virtualScreen.getSize().x / tlo_stolu.getSize().x;
    float scaleYtlo = (float)virtualScreen.getSize().y / tlo_stolu.getSize().y;
    tlo.setScale(scaleXtlo, scaleYtlo);
    // Skalowanie scian na cały ekran:
    float scaleXsciany = (float)virtualScreen.getSize().x / sciany_stol.getSize().x;
    float scaleYsciany = (float)virtualScreen.getSize().y / sciany_stol.getSize().y;
    sciany.setScale(scaleXsciany, scaleYsciany);

    // Jeden kontener obiektow gry
    std::vector<std::unique_ptr<GameObject>>entities;

    // Stworzenie dziur
    vector <float> pozycjedziurX = {320, 320, 24 ,26 , 614 ,614 };
    vector <float> pozycjedziurY = {23,  330, 27 ,328, 26, 330 };
    mult(pozycjedziurX, (float(virtualScreen.getSize().x)/window.getSize().x));
    mult(pozycjedziurY, (float(virtualScreen.getSize().y)/window.getSize().y));
    for (int i=0; i<6; i++)
    {
        float radi = 20*(float(virtualScreen.getSize().x)/window.getSize().x);
        sf::Vector2f position(pozycjedziurX[i],pozycjedziurY[i]);
        entities.push_back(std::make_unique<BilardHole>(radi, position));
    }

    // Stworzenie kul i ustawienie ich
    vector <float> pozycjebazoweX = {240,  162,162,162,162,162,  201,201,201,  220,220,  181,181,181,181,  400};
    vector <float> pozycjebazoweY = {180,  225,202,180,157,135,  157,202,180,  169,191,  146,214,169,191,  180};
    for (int i = 0; i < 16; i++) {
        float radi = 8 * (float(virtualScreen.getSize().x) / window.getSize().x);
        sf::Vector2f position(pozycjebazoweX[i] * (float(virtualScreen.getSize().x) / window.getSize().x),
                              pozycjebazoweY[i] * (float(virtualScreen.getSize().y) / window.getSize().y));

        auto ball = std::make_unique<BilardBall>(radi, position, i);
        if (i == 15) ball->setFillColor(sf::Color(255, 255, 255));
        else ball->setFillColor(sf::Color(180, 180, 180));

        entities.push_back(std::move(ball));
    }

    // Mapa ekranow
    std::map<GameState, std::unique_ptr<MenuScreen>> uiScreens;
    uiScreens[MENU] = std::make_unique<MainMenu>(rozdzielczosc);
    uiScreens[GAME_OVER] = std::make_unique<GameOverScreen>(rozdzielczosc);
    uiScreens[SHOP] = std::make_unique<ShopMenu>(rozdzielczosc);

    // Zmienne do ulepszeń
    float silaStrzalu = 1;
    float tarcieStoluGlobal = 1;
    float tarcieScianGlobal = 1;
    int maxStrzaly = 1;
    bool widocznoscCelu = true; // do testów, w grze zmienić na false żeby można było kupić

    // Zmienne do działania
    // int celPunktow = 1;
    int strzaly = 0;
    int lastHeldBall = -1;
    bool areBallsStationary = false;
    bool isLeftPressed = false;
    bool roundIsActive = false;
    bool isDragging = false;
    bool accelerateWhiteNow = false;
    float velc = 0.f;
    float dist_cent;
    sf::Vector2f addVelocity;
    sf::Vector2f p;
    sf::Clock clock;

    GameState currentState=MENU;
    MainMenu glowneMenu(rozdzielczosc);
    GameOverScreen ekranPrzegranej(rozdzielczosc);
    ShopMenu ekranSklepu(rozdzielczosc);
    LevelManager levelManager;
    HUD hud;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        // Wskaznik na biala bile
        BilardBall* whiteBall = nullptr;
        for (auto& obj : entities)
        {
            if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
            {
                if (bal->identifier == 15)
                {
                    whiteBall = bal;
                    break;
                }
            }
        }

        int aktualneMaxStrzaly = maxStrzaly; // Baza
        if (g_Stats.czyPosiada(101)) {
            aktualneMaxStrzaly += 3;
        }

        // Eventy:
        sf::Event event;
        while (window.pollEvent(event)) {
            // Zamknięcie okna
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                // Wymuszenie wielkości obrazu
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }

            // Myszka pozycja
            sf::Vector2i mouse_pos=sf::Mouse::getPosition(window);
            sf::Vector2f mouse_virtual_pos = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());
            p = mouse_virtual_pos;


            if(currentState==MENU || currentState == GAME_OVER || currentState == SHOP)
            {
                if (event.type == sf::Event::MouseMoved)
                {
                    uiScreens[currentState]->updateHover(mouse_virtual_pos);
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int akcja = uiScreens[currentState]->handleClick(mouse_virtual_pos);

                    if (akcja == 1)
                    {
                        std::cout << "debug - Przycisk PLAY klikniety" << std::endl;
                        g_Stats.ResetujGre();
                        resetBoard(entities, pozycjebazoweX, pozycjebazoweY, strzaly);
                        levelManager.przygotujRunde();
                        roundIsActive = true; currentState = PLAYING;
                    }
                    else if (akcja == 2) { std::cout << "Ustawienia"<<std::endl; }
                    else if (akcja == 3) { std::cout << "Zamkniecie okna"<<std::endl; window.close(); }
                    else if (akcja == 4)
                    {
                        g_Stats.ResetujGre();
                        resetCalejRozgrywki(entities, pozycjebazoweX, pozycjebazoweY, strzaly);
                        levelManager.przygotujRunde();
                        roundIsActive = true; currentState = PLAYING;
                    }
                    else if (akcja == 5) { currentState = MENU; }
                    else if (akcja == 6)
                    {
                        g_Stats.rundy++;
                        levelManager.przygotujRunde();
                        resetBoard(entities, pozycjebazoweX, pozycjebazoweY, strzaly);
                        roundIsActive = true; currentState = PLAYING;
                        std::cout << "Runda " << g_Stats.rundy << std::endl;
                    }
                }
            }
            else if (currentState == PLAYING)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    isLeftPressed = true;
                    for (auto& obj : entities)
                    {
                        if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                        {
                            if (square(bal->getPosition().x - p.x) + square(bal->getPosition().y - p.y) < square(bal->getRadius()))
                            {
                                bal->Held = true;
                                if (bal->identifier == 15 && areBallsStationary && strzaly < aktualneMaxStrzaly)
                                {
                                    lastHeldBall = bal->identifier;
                                }
                            } else
                            {
                                bal->Held = false;
                            }
                        }
                    }
                }
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                {
                    isLeftPressed = false;
                    lastHeldBall = -1;
                    if (isDragging) accelerateWhiteNow = true;
                    isDragging = false;
                }
                if (event.type == sf::Event::MouseMoved && isLeftPressed)
                {
                    for (auto& obj : entities)
                    {
                        if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                        {
                            float d_cent = square(bal->getPosition().x - p.x) + square(bal->getPosition().y - p.y);
                            if (d_cent < square(bal->getRadius()))
                            {
                                bal->Held = true;
                                if (bal->identifier == 15 && areBallsStationary && strzaly < aktualneMaxStrzaly)
                                {
                                    lastHeldBall = bal->identifier;
                                }
                            } else
                            {
                                bal->Held = false;
                            }
                        }
                    }
                }
            }
        }
            if(currentState==PLAYING)
            {
                // Check mozliwosci strzalu
                areBallsStationary = true;
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                    {
                        if (!bal->stationary()) areBallsStationary = false;
                    }
                }
                // Zakończenie rundy
                if( areBallsStationary && strzaly >= aktualneMaxStrzaly )
                {
                    g_Stats.punktyGlobalnie += g_Stats.punktyTejRundy;
                    if( g_Stats.punktyTejRundy >= levelManager.celPunktow )
                    {
                        // Win
                        roundIsActive = false;
                        currentState=SHOP;
                        if (auto* sklep = dynamic_cast<ShopMenu*>(uiScreens[SHOP].get()))
                        {
                            sklep->OdswiezPrzedmioty();
                        }
                        std::cout<<"Sklep"<<std::endl;
                    }
                    else
                    {
                        // Lose
                        roundIsActive = false;
                        auto* goScreen = dynamic_cast<GameOverScreen*>(uiScreens[GAME_OVER].get());
                        if(goScreen) goScreen->updateStats(g_Stats);
                        currentState=GAME_OVER;
                        std::cout<<"Przegrana"<<std::endl;
                    }
                }

                // Sprawdzanie czy celujemy
                if (whiteBall)
                {
                    dist_cent = square(whiteBall->getPosition().x - p.x) + square(whiteBall->getPosition().y - p.y);
                    dist_cent = sqrt(dist_cent);
                    if (lastHeldBall == 15 && dist_cent > whiteBall->getRadius() + 4 )
                    {
                        isDragging = true;
                    }
                    else
                    {
                        isDragging = false;
                    }

                    // Strzał w białą bilę
                    velc = clamp(dist_cent,5.f,100.f);
                    if(accelerateWhiteNow)
                    {
                        accelerateWhiteNow = false;
                        //cout<<"velc = "<<velc<<endl;
                        addVelocity = sf::Vector2f(0,0);
                        addVelocity.x = (10*velc/dist_cent)*(whiteBall->getPosition().x-p.x) ;
                        addVelocity.y = (10*velc/dist_cent)*(whiteBall->getPosition().y-p.y) ;
                        float aktualnaSila = silaStrzalu;
                        if (g_Stats.czyPosiada(102)) {
                            aktualnaSila *= 1.30f; // + 30% siły
                        }
                        whiteBall->setVelocity(addVelocity * aktualnaSila);
                        velc = 0;
                        strzaly ++;
                        g_Stats.strzalyGlobalnie++;
                        cout<<"strzal #"<<strzaly<<endl;
                    }
                }

                // Fizyka w jednym wektorze
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get())) bal->cleanBounces();
                }
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                    {
                        if (!bal->Put) bal->kolizjeKul(elapsed, entities, tarcieScianGlobal, tarcieStoluGlobal);
                    }
                }
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                    {
                        if (!bal->Put) bal->kolizjeScian(elapsed, tarcieScianGlobal, tarcieStoluGlobal);
                    }
                }
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                    {
                        // Zamiast starej funkcji, sprawdzamy dziury z uwzględnieniem blokad:
                        if (!bal->Put)
                        {
                            int indeksDziury = 0;
                            for (auto& holeObj : entities)
                            {
                                if (auto* hol = dynamic_cast<BilardHole*>(holeObj.get()))
                                {
                                    if (!levelManager.czyDziuraZablokowana(indeksDziury) &&
                                        diff(bal->getPosition(), hol->getPosition()) < bal->getRadius() + hol->getRadius())
                                    {
                                        bal->ballPut();
                                        break;
                                    }
                                    indeksDziury++;
                                }
                            }
                        }
                    }
                }
                // Update
                // Update z uwzględnieniem tarcia Bossów
                for (auto& obj : entities)
                {
                    if (auto* bal = dynamic_cast<BilardBall*>(obj.get()))
                    {
                        float mnoznikTarcia = levelManager.zmniejszoneTarcieAktywne ? levelManager.tarcieStoluGlobal : 1.f;
                        if (levelManager.obecnyBoss == BossType::Mud)
                        {
                            for (const auto& plama : levelManager.plamyBlota)
                            {
                                if (diff(bal->getPosition(), plama.getPosition()) <= plama.getRadius())
                                {
                                    mnoznikTarcia = std::max(mnoznikTarcia, levelManager.mnoznikTarciaBlota);
                                    break;
                                }
                            }
                        }
                        bal->tarcie = bal->tarcieBaza * mnoznikTarcia;
                    }
                    obj->update(elapsed);
                }
            }

            // Przygotowanie ekranu do renderowania:
            virtualScreen.clear(currentState==MENU ? sf::Color::White : sf::Color::Black);

            virtualScreen.draw(tlo);
            if ((currentState == PLAYING || currentState == GAME_OVER) && levelManager.obecnyBoss == BossType::Ice) {
                virtualScreen.draw(lodVisual);
            }

            if (currentState == PLAYING || currentState == GAME_OVER) {

                // Rysowanie błota pod bilami
                if ((currentState == PLAYING || currentState == GAME_OVER) && levelManager.obecnyBoss == BossType::Mud) {
                    for (const auto& plama : levelManager.plamyBlota) virtualScreen.draw(plama);
                }

                // Rysowanie zablokowanych dziur
                int i_dziura = 0;
                for (auto& obj : entities) {
                    if (auto* hol = dynamic_cast<BilardHole*>(obj.get())) {

                        obj->draw(virtualScreen); // Najpierw rysujemy normalną, czarną dziurę

                        // Jeśli system mówi, że ta dziura ma być zablokowana przez Bossa:
                        if (levelManager.czyDziuraZablokowana(i_dziura)) {

                            // Blokada dziury
                            sf::CircleShape blokada(hol->getRadius());
                            blokada.setOrigin(hol->getRadius(), hol->getRadius());
                            blokada.setPosition(hol->getPosition()); // Ustawiamy dokładnie w miejscu dziury
                            blokada.setFillColor(sf::Color(95, 95, 95)); // Szary kolor
                            blokada.setOutlineColor(sf::Color(170, 30, 30)); // Czerwona ramka ostrzegawcza
                            blokada.setOutlineThickness(2.f);

                            virtualScreen.draw(blokada); // Rysujemy blokadę na dziurze
                        }
                        i_dziura++;
                    }
                }

                virtualScreen.draw(sciany);

                if (roundIsActive || currentState == GAME_OVER) {
                    for (auto& obj : entities) {
                        if (dynamic_cast<BilardBall*>(obj.get())) obj->draw(virtualScreen);
                    }

                    // Linia celownika
                    if (currentState == PLAYING && isDragging && whiteBall)
                    {
                        if(widocznoscCelu)
                        {
                            float mnoznikCelownika = 2.0f; // Bazowa długość
                            if (g_Stats.czyPosiada(100)) {
                                mnoznikCelownika *= 2.0f; // Snajper: 2x dłuższa linia
                            }

                            sf::Vector2f plin;
                            plin.x = whiteBall->getPosition().x + (2 * velc / dist_cent) * (whiteBall->getPosition().x - p.x);
                            plin.y = whiteBall->getPosition().y + (2 * velc / dist_cent) * (whiteBall->getPosition().y - p.y);
                            RysujLinie(virtualScreen, whiteBall->getPosition(), plin, 2.f, sf::Color::White);
                        }


                    // Rysowanie kija
                    // Obliczamy wektor od białej bili do myszki
                    sf::Vector2f kierunekDoMyszki = p - whiteBall->getPosition();

                    // Obzliczamy kąt w stopniach (używamy atan2)
                    float katKija = std::atan2(kierunekDoMyszki.y, kierunekDoMyszki.x) * 180.f / 3.14159265f;
                    sprKij.setRotation(katKija);

                    // Efekt "odciągania" kija przy nabieraniu siły
                    // Normalizujemy wektor (robimy z niego długość 1) i mnożymy przez velc (siłę naciągu)
                    float dlugosc = std::sqrt(square(kierunekDoMyszki.x) + square(kierunekDoMyszki.y));
                    if (dlugosc > 0) {
                        sf::Vector2f znormalizowany = kierunekDoMyszki / dlugosc;
                        // Odsuwamy kij od bili o wartość siły 'velc'
                        // Dodajemy też promień bili, żeby kij w nią nie wchodził
                        sf::Vector2f pozycjaKija = whiteBall->getPosition() + znormalizowany * (velc + whiteBall->getRadius());
                        sprKij.setPosition(pozycjaKija);
                    }
                    else
                    {
                        sprKij.setPosition(whiteBall->getPosition());
                    }

                    virtualScreen.draw(sprKij);
                    }
                }
            }

            // Rysujemy odpowiednie menu (jeśli w nim jesteśmy)
            if (currentState != PLAYING) {
                uiScreens[currentState]->draw(virtualScreen);
            }

            // Hud
            if (currentState == PLAYING) {
                std::string etykietaRundy = "RUNDA: " + std::to_string(g_Stats.rundy);
                if (levelManager.obecnyBoss != BossType::None) {
                    etykietaRundy = "Boss#" + std::to_string(g_Stats.rundy / 5);
                }
                hud.draw(virtualScreen, aktualneMaxStrzaly, strzaly, levelManager.celPunktow, etykietaRundy);
            }

            virtualScreen.display();
            sf::Sprite screenSprite(virtualScreen.getTexture());
            float scaleX = (float)window.getSize().x / virtualScreen.getSize().x;
            float scaleY = (float)window.getSize().y / virtualScreen.getSize().y;
            screenSprite.setScale(scaleX, scaleY);
            window.clear();
            window.draw(screenSprite);
            window.display();
    }


    return 0;
}
