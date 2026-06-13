#include "ShopMenu.h"
#include "Globals.h"
#include "Upgrades.h"
#include <iostream>
#include<algorithm>
#include<random>

ShopMenu::ShopMenu(std::pair<int,int> res) {
    // Tła i panele
    // Ładowanie ceglanego tła (640x360)
    if (texTloCegly.loadFromFile("assets//shop4.png")) {
        texTloCegly.setSmooth(false);
        sprTloCegly.setTexture(texTloCegly);
    }

    // Ładowanie drewnianego panelu na środek
    if (texDrewnoSrodek.loadFromFile("assets//shop_srodek2.png")) {
        texDrewnoSrodek.setSmooth(false);
        sprDrewnoSrodek.setTexture(texDrewnoSrodek);

        // Automatycznie liczy środek ekranu (640) minus szerokość Twojego PNG
        float srodekX = (res.first - sprDrewnoSrodek.getGlobalBounds().width) / 2.0f;
        sprDrewnoSrodek.setPosition(srodekX, 0);
    }

    // Tekst
    if(font.loadFromFile("assets//PublicPixel.ttf"))
    {
        titleText.setFont(font); titleText.setString("SKLEP");
        titleText.setCharacterSize(30); titleText.setFillColor(sf::Color::White);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width/2.0f, tb.top + tb.height/2.0f);
        titleText.setPosition(res.first/2.0f, 40);

        inventoryText.setFont(font); inventoryText.setString("EKWIPUNEK");
        inventoryText.setCharacterSize(14); inventoryText.setFillColor(sf::Color::Yellow);
        sf::FloatRect ib = inventoryText.getLocalBounds();
        inventoryText.setOrigin(ib.left + ib.width/2.0f, ib.top + ib.height/2.0f);
        inventoryText.setPosition(res.first/2.0f, 90);

        // monety
        textMonety.setFont(font);
        textMonety.setCharacterSize(14);
        textMonety.setFillColor(sf::Color::Yellow);
        // Ustawiamy w lewym górnym rogu ekranu
        textMonety.setPosition(20.f, 20.f);

        textKosztRefresha.setFont(font);
        textKosztRefresha.setCharacterSize(10);
        textKosztRefresha.setFillColor(sf::Color::Yellow);

        // Ustawienia pływającego okienka
        tooltipName.setFont(font); tooltipName.setCharacterSize(12); tooltipName.setFillColor(sf::Color::Yellow);
        tooltipDesc.setFont(font); tooltipDesc.setCharacterSize(10); tooltipDesc.setFillColor(sf::Color::White);
        tooltipPrice.setFont(font); tooltipPrice.setCharacterSize(12); tooltipPrice.setFillColor(sf::Color::Green);
    }

    if (texKijSklep.loadFromFile("assets//kij.png")) {
        texKijSklep.setSmooth(false);
        sprKijSklep.setTexture(texKijSklep);
        sf::FloatRect bounds = sprKijSklep.getLocalBounds();
        sprKijSklep.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        // Skala 0.45f żeby kij zmieścił się idealnie w poziomie na szerokość karty
        sprKijSklep.setScale(0.45f, 0.45f);
    }

    // Ustawienie grafiki bili do sklepu
    sprBilaSklep.setRadius(16.f);
    sprBilaSklep.setOrigin(16.f, 16.f);
    sprBilaSklep.setFillColor(sf::Color(180, 180, 180)); // Szary kolor jak bazowe bile
    sprBilaSklep.setOutlineThickness(2.f);
    sprBilaSklep.setOutlineColor(sf::Color(80, 80, 80)); // Ciemniejsza obwódka

    // Pływające okienko tło
    tooltipBg.setFillColor(sf::Color(0, 0, 0, 220));
    tooltipBg.setOutlineThickness(2);
    tooltipBg.setOutlineColor(sf::Color::White);

    // Ladowanie grafik sklepu
    texBoxKijNorm.loadFromFile("assets//SHOP_kije_tlo_normal.png");
    texBoxKijHov.loadFromFile("assets//SHOP_kije_tlo_hover.png");
    texBoxBilaNorm.loadFromFile("assets//SHOP_bile_tlo_normal.png");
    texBoxBilaHov.loadFromFile("assets//SHOP_bile_tlo_hover.png");

    if(texKijSrodek.loadFromFile("assets//SHOP_moj_kij_tlo_normal.png")) {
        sprKijSrodek.setTexture(texKijSrodek);
        sprKijSrodek.setOrigin(texKijSrodek.getSize().x / 2.0f, texKijSrodek.getSize().y / 2.0f);
        sprKijSrodek.setPosition(res.first/2.0f, 140);
    }

    if(texTrojkatSrodek.loadFromFile("assets//SHOP_moje_bile_tlo_normal.png")) {
        sprTrojkatSrodek.setTexture(texTrojkatSrodek);
        sprTrojkatSrodek.setOrigin(texTrojkatSrodek.getSize().x / 2.0f, texTrojkatSrodek.getSize().y / 2.0f);
        sprTrojkatSrodek.setPosition(res.first/2.0f, 220);
    }

    if(texBtnRefreshNorm.loadFromFile("assets//SHOP_refresh_normal.png") && texBtnRefreshHov.loadFromFile("assets//SHOP_refresh_hover.png")) {
        sprBtnRefresh.setTexture(texBtnRefreshNorm);
        sprBtnRefresh.setOrigin(texBtnRefreshNorm.getSize().x / 2.0f, texBtnRefreshNorm.getSize().y / 2.0f);
        // Ustawiamy idealnie na środku osi X, tuż nad przyciskiem Next Round
        sprBtnRefresh.setPosition(res.first / 2.0f, res.second - 85);
    }

    if(texBtnNextNorm.loadFromFile("assets//SHOP_continue_normal.png") && texBtnNextHov.loadFromFile("assets//SHOP_continue_hover.png")) {
        sprBtnNext.setTexture(texBtnNextNorm);
        sprBtnNext.setOrigin(texBtnNextNorm.getSize().x / 2.0f, texBtnNextNorm.getSize().y / 2.0f);
        sprBtnNext.setPosition(res.first/2.0f, res.second - 40);
    }



    OdswiezPrzedmioty();

    // Upgrady
    std::vector<Upgrade> kijeBaza;
    std::vector<Upgrade> bileBaza;
    for(auto& u : PelnaBazaUlepszen) {
        if(u.rodzaj == 0) kijeBaza.push_back(u);
        else bileBaza.push_back(u);
    }

    // Losowanie upgradow
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(kijeBaza.begin(), kijeBaza.end(), g);
    std::shuffle(bileBaza.begin(), bileBaza.end(), g);
    // Wybieramy pierwsze 3 kije i 4 bile
    for(int i = 0; i < 3; i++) wylosowaneKije.push_back(kijeBaza[i]);
    for(int i = 0; i < 4; i++) wylosowaneBile.push_back(bileBaza[i]);

    btnNextText.setFont(font); btnNextText.setString("NASTEPNA RUNDA");
    btnNextText.setCharacterSize(10); btnNextText.setFillColor(sf::Color::White);
    sf::FloatRect nb = btnNextText.getLocalBounds();
    btnNextText.setOrigin(nb.left + nb.width/2.0f, nb.top + nb.height/2.0f);
    btnNextText.setPosition(res.first/2.0f, res.second - 40);

    // Bile
    darkOverlay.setSize(sf::Vector2f(res.first, res.second));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 230)); // Mocne przyciemnienie

    closePromptText.setFont(font); closePromptText.setString("Kliknij gdziekolwiek, aby wrocic");
    closePromptText.setCharacterSize(12); closePromptText.setFillColor(sf::Color(150,150,150));
    sf::FloatRect cb = closePromptText.getLocalBounds();
    closePromptText.setOrigin(cb.left + cb.width/2.0f, cb.top + cb.height/2.0f);
    closePromptText.setPosition(res.first/2.0f, res.second - 30);

    // Generowanie trójkąta z 15 okręgów na środku
    float radius = 15.0f;
    float spacing = 35.0f;
    float startX = res.first / 2.0f;
    float startY_triangle = 100.0f;
    const std::vector<int> rackOrder = {
        0,
        9, 10,
        6, 8, 7,
        11, 13, 14, 12,
        5, 4, 3, 2, 1
    };

    int ballCount = 0;
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col <= row; col++)
        {
            sf::CircleShape ball(radius);
            ball.setOrigin(radius, radius);
            float xPos = startX + (col * spacing) - (row * spacing / 2.0f);
            float yPos = startY_triangle + (row * spacing * 0.866f); // 0.866 to sin(60st)

            ball.setPosition(xPos, yPos);
            ball.setFillColor(sf::Color(180, 180, 180));

            inventoryBalls.push_back(ball);
            inventoryBallIds.push_back(rackOrder[ballCount]);
            ballCount++;
        }
    }

}

void ShopMenu::OdswiezPrzedmioty()
{
    ballButtons.clear();
    cueButtons.clear();
    hoveredItem = nullptr;
    pendingBallUpgradeActive = false;
    pendingBallUpgradeId = -1;
    pendingBallUpgradeCena = 0;
    pendingBallUpgradeNazwa.clear();
    showTooltip = false;
    currentSubState = SHOP_MAIN;

    wylosowaneKije.clear();
    wylosowaneBile.clear();


    std::vector<Upgrade> dostepneKije;
    std::vector<Upgrade> dostepneBile;

    // Krok A: Filtrujemy bazę na dwie kategorie i ODRZUCAMY kupione
    for (int i = 0; i < PelnaBazaUlepszen.size(); i++) {
        // Zabezpieczenie 1: Pomijamy przedmioty, które gracz już ma w ekwipunku
        if (g_Stats.czyPosiada(PelnaBazaUlepszen[i].id)) {
            continue;
        }

        if (PelnaBazaUlepszen[i].rodzaj == 0) {
            dostepneKije.push_back(PelnaBazaUlepszen[i]);
        } else if (PelnaBazaUlepszen[i].rodzaj == 1) {
            dostepneBile.push_back(PelnaBazaUlepszen[i]);
        }
    }

    // Krok B: Losujemy 3 kije (bez powtórzeń)
    for (int i = 0; i < 3; i++) {
        if (dostepneKije.empty()) break; // Zabezpieczenie: przerywamy, jeśli gracz wykupił już wszystkie kije z gry

        int losowyIndex = rand() % dostepneKije.size();
        wylosowaneKije.push_back(dostepneKije[losowyIndex]);

        // Zabezpieczenie 2: Usuwamy kij z tymczasowej puli, żeby nie wylosować go drugi raz w tym samym sklepie
        dostepneKije.erase(dostepneKije.begin() + losowyIndex);
    }

    // Krok C: Losujemy 4 bile (bez powtórzeń)
    for (int i = 0; i < 4; i++) {
        if (dostepneBile.empty()) break; // Zabezpieczenie przed brakiem bil

        int losowyIndex = rand() % dostepneBile.size();
        wylosowaneBile.push_back(dostepneBile[losowyIndex]);

        // Usuwamy bilę z tymczasowej puli
        dostepneBile.erase(dostepneBile.begin() + losowyIndex);
    }


    // przyciski sklep
    // tworzenie przyciskow kiji (Lewa strona)
    float startY_kije = 120.0f; //

    // SFML sam wylicza połowę odległości od krawędzi ekranu do desek!
    float srodekLewej = sprDrewnoSrodek.getPosition().x / 2.0f;

    for(int i = 0; i < wylosowaneKije.size(); i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(140, 50));
        btn.shape.setOrigin(70, 25);
        btn.shape.setPosition(srodekLewej, startY_kije + (i * 70));
        btn.shape.setFillColor(sf::Color::Transparent);

        btn.nazwa = wylosowaneKije[i].nazwa;
        btn.opis = wylosowaneKije[i].opis;
        btn.cena = wylosowaneKije[i].cena;
        btn.id = wylosowaneKije[i].id;
        btn.kupiony=false;

        btn.sprite.setTexture(texBoxKijNorm);
        if (texBoxKijNorm.getSize().x > 0) {
            btn.sprite.setOrigin(texBoxKijNorm.getSize().x / 2.0f, texBoxKijNorm.getSize().y / 2.0f);
            btn.sprite.setScale(140.0f / texBoxKijNorm.getSize().x, 50.0f / texBoxKijNorm.getSize().y);
        }
        // Ustawiamy przycisk idealnie na dynamicznie wyliczonym środku
        btn.sprite.setPosition(srodekLewej, startY_kije + (i * 70));

        cueButtons.push_back(btn);
    }

    // przyciski bile
    // tworzenie przyciskow bil (Prawa strona)
    for(int i = 0; i < wylosowaneBile.size(); i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(60, 60));
        btn.shape.setOrigin(30, 30);

        int col = i % 2; // 0 lub 1
        int row = i / 2; // 0 lub 1

        // 490 i 570 to pozycje X (środek między nimi to dokładnie 530!)
        float pozX = 490.0f + (col * 80.0f);
        float pozY = 140.0f + (row * 80.0f);

        btn.shape.setPosition(pozX, pozY);
        btn.shape.setFillColor(sf::Color::Transparent);

        btn.nazwa = wylosowaneBile[i].nazwa;
        btn.opis = wylosowaneBile[i].opis;
        btn.cena = wylosowaneBile[i].cena;
        btn.id = wylosowaneBile[i].id;
        btn.kupiony=false;

        btn.sprite.setTexture(texBoxBilaNorm);
        if (texBoxBilaNorm.getSize().x > 0) {
            btn.sprite.setOrigin(texBoxBilaNorm.getSize().x / 2.0f, texBoxBilaNorm.getSize().y / 2.0f);
            // Wymuszamy rozmiar 60x60
            btn.sprite.setScale(60.0f / texBoxBilaNorm.getSize().x, 60.0f / texBoxBilaNorm.getSize().y);
        }
        btn.sprite.setPosition(pozX, pozY); // Ustawiamy bezpośrednio

        ballButtons.push_back(btn);
    }
}


void ShopMenu::updateHover(sf::Vector2f mousePos)
{
    showTooltip = false; // Domyślnie chowamy tooltip
    hoveredItem = nullptr;

    if (currentSubState == SHOP_MAIN) {
        // Sprawdzamy Kije do kupienia
        for(auto& btn : cueButtons)
        {
            if(btn.shape.getGlobalBounds().contains(mousePos))
            {
                if (!btn.kupiony) {
                    btn.sprite.setTexture(texBoxKijHov);
                }
                hoveredItem = &btn;
                tooltipName.setString(btn.nazwa);
                tooltipDesc.setString(btn.opis);
                tooltipPrice.setString("Cena: " + std::to_string(btn.cena));
                showTooltip = true;
            }
            else
            {
                if(btn.kupiony) {
                    btn.shape.setFillColor(sf::Color(50, 150, 50, 160));
                    btn.shape.setOutlineColor(sf::Color::Transparent);
                }
                else {
                    btn.sprite.setTexture(texBoxKijNorm);
                }
            }
        }

        // Sprawdzamy Bile do kupienia
        for(auto& btn : ballButtons)
        {
            if(btn.shape.getGlobalBounds().contains(mousePos))
            {
                if (!btn.kupiony) {
                    btn.sprite.setTexture(texBoxBilaHov);
                }
                hoveredItem = &btn;
                tooltipName.setString(btn.nazwa);
                tooltipDesc.setString(btn.opis);
                tooltipPrice.setString("Cena: " + std::to_string(btn.cena));
                showTooltip = true;
            }
            else
            {
                if(btn.kupiony) {
                    btn.shape.setFillColor(sf::Color(50, 150, 50, 160));
                    btn.shape.setOutlineColor(sf::Color::Transparent);
                }
                else {
                    btn.sprite.setTexture(texBoxBilaNorm);
                }
            }
        }

        // Sprawdzanie ikony trójkąta na środku
        if(sprTrojkatSrodek.getGlobalBounds().contains(mousePos))
        {
            sprTrojkatSrodek.setColor(sf::Color(200, 200, 200));
            tooltipName.setString("Twoje Bile"); tooltipDesc.setString("Kliknij, aby sprawdzic"); tooltipPrice.setString("");
            showTooltip = true;
        } else {
            sprTrojkatSrodek.setColor(sf::Color::White);
        }

        // Sprawdzanie obecnego kija na środku
        if(sprKijSrodek.getGlobalBounds().contains(mousePos))
        {
            extern std::vector<Upgrade> PelnaBazaUlepszen; // Dostęp do bazy

            tooltipName.setString("Twoj Kij");

            // Budujemy wielolinijkowy tekst z ulepszeniami
            std::string listaUlepszen = "";
            int iloscKijow = 0;

            for (int id : g_Stats.posiadaneUpgradeID)
            {
                if (id < 200) // Wyłapujemy tylko kije
                {
                    for (const auto& upg : PelnaBazaUlepszen) {
                        if (upg.id == id) {
                            listaUlepszen += "- " + upg.nazwa + "\n";
                            iloscKijow++;
                            break;
                        }
                    }
                }
            }

            // Jeśli nie mamy nic, dajemy domyślny opis
            if (iloscKijow == 0) {
                listaUlepszen = "Brak ulepszen. Podstawowy sprzet.";
            }

            tooltipDesc.setString(listaUlepszen);
            tooltipPrice.setString("");
            showTooltip = true;
        }

        if(sprBtnNext.getGlobalBounds().contains(mousePos)) sprBtnNext.setTexture(texBtnNextHov);
        else sprBtnNext.setTexture(texBtnNextNorm);

        if(sprBtnRefresh.getGlobalBounds().contains(mousePos)) sprBtnRefresh.setTexture(texBtnRefreshHov);
        else sprBtnRefresh.setTexture(texBtnRefreshNorm);

    }
    else if (currentSubState == SHOP_BALL_INVENTORY)
    {
        extern std::vector<Upgrade> PelnaBazaUlepszen; // Dostęp do bazy danych

        for (int i = 0; i < inventoryBalls.size(); i++)
        {
            if (inventoryBalls[i].getGlobalBounds().contains(mousePos))
            {
                int ballId = inventoryBallIds[i];
                tooltipName.setString("Bila " + std::to_string(ballId + 1)); // Bila 1, Bila 2...

                // Budujemy listę posiadanych ulepszeń dla tej konkretnej bili
                std::string listaBil = "";
                for (int id : g_Stats.ulepszeniaBil[ballId]) {
                    for (const auto& upg : PelnaBazaUlepszen) {
                        if (upg.id == id) {
                            listaBil += "- " + upg.nazwa + "\n";
                            break;
                        }
                    }
                }

                if (listaBil.empty()) listaBil = "Brak ulepszen. Podstawowa masa.";

                tooltipDesc.setString(listaBil);
                tooltipPrice.setString("");
                showTooltip = true;
            }
        }
    }

    // Aktualizacja pozycji tooltipa, jeśli ma być widoczny
    if (showTooltip)
    {
        float tooltipX = mousePos.x + 15;
        float tooltipY = mousePos.y + 15;

        float maxWidth = std::max({tooltipName.getLocalBounds().width, tooltipDesc.getLocalBounds().width, tooltipPrice.getLocalBounds().width});
        float descHeight = tooltipDesc.getLocalBounds().height;

        // 35 to miejsce na tytuł, górny i dolny margines
        float totalHeight = 35 + descHeight;

        // Jeśli jest cena, dodajemy na nią kolejne 20 pikseli
        if (!tooltipPrice.getString().isEmpty()) {
            totalHeight += 20;
        }

        // 2. ZABEZPIECZENIA PRZED UCIECZKĄ Z EKRANU
        if (tooltipX + maxWidth + 10 > 640)
        {
            tooltipX = mousePos.x - maxWidth - 25;
        }

        if (tooltipY + totalHeight + 10 > 360)
        {
            // Przesuwamy w górę o całą CAŁĄ nową wysokość
            tooltipY = mousePos.y - totalHeight - 15;
        }
        if (tooltipY < 5)
        {
            tooltipY = 5;
        }

        // 3. USTAWIENIE POZYCJI ELEMENTÓW
        tooltipBg.setPosition(tooltipX, tooltipY);
        tooltipBg.setSize(sf::Vector2f(maxWidth + 10, totalHeight));

        tooltipName.setPosition(tooltipX + 5, tooltipY + 5);
        tooltipDesc.setPosition(tooltipX + 5, tooltipY + 25); // Zawsze zaraz pod tytułem

        // Jeśli jest cena, musi "uciec" pod rozciągnięty opis
        if (!tooltipPrice.getString().isEmpty()) {
            tooltipPrice.setPosition(tooltipX + 5, tooltipY + 30 + descHeight);
        }
    }
}

int ShopMenu::handleClick(sf::Vector2f mousePos)
{
    ShopSubState stateAtClick = currentSubState;

    if (stateAtClick == SHOP_MAIN)
    {
        // Przejście do widoku szczegółów bil
        if (sprTrojkatSrodek.getGlobalBounds().contains(mousePos))
        {
            currentSubState = SHOP_BALL_INVENTORY;
            return 0; // Nie wychodzimy ze sklepu, zmieniamy tylko stan
        }

        if (sprBtnNext.getGlobalBounds().contains(mousePos)) {
            return 6; // Next Round
        }

        ShopButton* clickedItem = nullptr;
        for (auto& btn : cueButtons) {
            if (btn.shape.getGlobalBounds().contains(mousePos)) {
                clickedItem = &btn;
                break;
            }
        }
        if (clickedItem == nullptr) {
            for (auto& btn : ballButtons) {
                if (btn.shape.getGlobalBounds().contains(mousePos)) {
                    clickedItem = &btn;
                    break;
                }
            }
        }

        if (clickedItem != nullptr) {
            if (clickedItem->kupiony) {
                std::cout << "Sklep - Wykupiles to w tym losowaniu" << std::endl;
            }
            else if (clickedItem->id >= 200)
            {
                // --- KUPUJEMY BILĘ: Zatrzymujemy transakcję i otwieramy trójkąt ---
                pendingBallUpgradeActive = true;
                pendingBallUpgradeId = clickedItem->id;
                pendingBallUpgradeCena = clickedItem->cena;
                pendingBallUpgradeNazwa = clickedItem->nazwa;
                currentSubState = SHOP_BALL_INVENTORY;
            }
            else if (g_Stats.kupUlepszenie(clickedItem->cena, clickedItem->id))
            {
                // --- KUPUJEMY KIJ: Płacimy i dostajemy od razu ---
                std::cout << "Sklep - Kupiono KIJ: " << clickedItem->nazwa << std::endl;
                clickedItem->kupiony = true;
                clickedItem->shape.setFillColor(sf::Color(50, 150, 50));
            }
            else
            {
                std::cout << "Sklep - Za malo monet!" << std::endl;
            }
        }
    }
    else if (stateAtClick == SHOP_BALL_INVENTORY) {
        bool clickedOnBall = false;

        for (int i = 0; i < inventoryBalls.size(); i++) {
            if (inventoryBalls[i].getGlobalBounds().contains(mousePos)) {
                clickedOnBall = true;

                int ballId = inventoryBallIds[i];

                // Jeśli jesteśmy tu po to, żeby przypisać ulepszenie ze sklepu:
                if (pendingBallUpgradeActive) {

                    // 1. Sprawdzamy, czy ta konkretna bila MA JUŻ to ulepszenie
                    bool maJuzTo = false;
                    for (int posiadaneID : g_Stats.ulepszeniaBil[ballId]) {
                        if (posiadaneID == pendingBallUpgradeId) maJuzTo = true;
                    }

                    if (maJuzTo) {
                        std::cout << "Ta bila ma juz to ulepszenie! Wybierz inna." << std::endl;
                        break; // Przerywamy akcję, gracz nadal może wybrać inną bilę
                    }

                    // 2. Pobieramy opłatę i przydzielamy ulepszenie!
                    if (g_Stats.kupUlepszenie(pendingBallUpgradeCena, pendingBallUpgradeId)) {
                        g_Stats.ulepszeniaBil[ballId].push_back(pendingBallUpgradeId); // Dodajemy do szufladki tej bili

                        std::cout << "Bila " << ballId + 1 << " dostala: " << pendingBallUpgradeNazwa << "!" << std::endl;

                        for (auto& btn : ballButtons) {
                            if (btn.id == pendingBallUpgradeId) {
                                btn.kupiony = true;
                                btn.shape.setFillColor(sf::Color(50, 150, 50));
                                break;
                            }
                        }
                    } else {
                        std::cout << "Za malo monet, aby przypisac do bili!" << std::endl;
                    }

                    // Zamykamy trójkąt, wracamy do sklepu
                    pendingBallUpgradeActive = false;
                    pendingBallUpgradeId = -1;
                    pendingBallUpgradeCena = 0;
                    pendingBallUpgradeNazwa.clear();
                    currentSubState = SHOP_MAIN;
                }
                break;
            }
        }

        // Anulowanie zakupu, jeśli gracz kliknie w tło (poza bilami)
        if (!clickedOnBall) {
            pendingBallUpgradeActive = false;
            pendingBallUpgradeId = -1;
            pendingBallUpgradeCena = 0;
            pendingBallUpgradeNazwa.clear();
            currentSubState = SHOP_MAIN;
        }
    }

    if (stateAtClick == SHOP_MAIN && currentSubState == SHOP_MAIN && sprBtnRefresh.getGlobalBounds().contains(mousePos))
    {
        if (g_Stats.monety >= kosztRefresha) {
            g_Stats.monety -= kosztRefresha;
            OdswiezPrzedmioty(); // nowe itemy
            std::cout << "Kupiono Reroll za " << kosztRefresha << " monet!" << std::endl;
        }
        else
        {
            std::cout << "Brak monet na Reroll!" << std::endl;
        }
    }
    return 0;
}

void ShopMenu::draw(sf::RenderTexture& target) {
    target.draw(sprTloCegly);

    textMonety.setString("Monety: " + std::to_string(g_Stats.monety));
    target.draw(textMonety);

    if (currentSubState == SHOP_MAIN) {
        target.draw(sprDrewnoSrodek);
        target.draw(titleText); target.draw(inventoryText);

        // rysowanie kijow
        for(auto& btn : cueButtons) {
            target.draw(btn.sprite);

            // Kij trafia idealnie na sam środek przycisku
            sprKijSklep.setPosition(btn.sprite.getPosition());
            target.draw(sprKijSklep);
        }

        // rysowanie bil
        // rysowanie bil
        for(auto& btn : ballButtons) {
            target.draw(btn.sprite);

            // Dynamiczna zmiana koloru na podstawie ID ulepszenia (identyczne kolory jak kropeczki na stole)
            sf::Color kolorBili(180, 180, 180); // domyślny szary, gdyby coś poszło nie tak
            switch (btn.id) {
                case 201: kolorBili = sf::Color(80, 80, 80); break;   // Kowadlo
                case 202: kolorBili = sf::Color(135, 206, 235); break; // Balon
                case 203: kolorBili = sf::Color(139, 69, 19); break;  // Kamien
                case 204: kolorBili = sf::Color(220, 20, 60); break;  // Bomba
                case 205: kolorBili = sf::Color(50, 205, 50); break;  // Sprezyna
                case 206: kolorBili = sf::Color(0, 255, 255); break;  // Lód
                case 207: kolorBili = sf::Color(255, 215, 0); break;  // Klej
                case 208: kolorBili = sf::Color(138, 43, 226); break; // Magnes
                case 209: kolorBili = sf::Color(255, 255, 255); break;// Duch
                case 210: kolorBili = sf::Color(255, 140, 0); break;  // Ogień
                case 211: kolorBili = sf::Color(0, 191, 255); break;  // Diament
                case 212: kolorBili = sf::Color(255, 223, 0); break;  // Gwiazda
                case 213: kolorBili = sf::Color(178, 34, 34); break;  // Cel
                case 214: kolorBili = sf::Color(218, 165, 32); break; // Korona
                case 215: kolorBili = sf::Color(255, 105, 180); break;// Skarbonka
                case 216: kolorBili = sf::Color(34, 139, 34); break;  // Inwestycja
                case 217: kolorBili = sf::Color(192, 192, 192); break;// Szkło
            }
            sprBilaSklep.setFillColor(kolorBili);

            // Bila trafia idealnie na sam środek przycisku
            sprBilaSklep.setPosition(btn.sprite.getPosition());
            target.draw(sprBilaSklep);
        }

        // Rysowanie środka
        target.draw(sprKijSrodek);

        // --- Rysowanie kija na środkowym panelu "Twój Kij" ---
        sprKijSklep.setPosition(sprKijSrodek.getPosition());
        target.draw(sprKijSklep);

        target.draw(sprTrojkatSrodek);

        target.draw(sprBtnNext);
        target.draw(sprBtnRefresh);

        // koszt reroll
        textKosztRefresha.setString("-" + std::to_string(kosztRefresha));
        sf::FloatRect kr = textKosztRefresha.getLocalBounds();
        textKosztRefresha.setOrigin(kr.left + kr.width / 2.0f, kr.top + kr.height / 2.0f);

        // Ustawiamy tekst lekko po prawej stronie od przycisku odświeżania
        textKosztRefresha.setPosition(sprBtnRefresh.getPosition().x + 70.f, sprBtnRefresh.getPosition().y);
        target.draw(textKosztRefresha);
    }
    else if (currentSubState == SHOP_BALL_INVENTORY) {
        // Widok inwentarza bil
        target.draw(sprDrewnoSrodek);
        target.draw(darkOverlay); // Przyciemniamy wszystko

        for (auto& ball : inventoryBalls) target.draw(ball);
        target.draw(closePromptText);
    }

    // Pływające okienko rysujemy NA SAMEJ GÓRZE wszystkiego
    if (showTooltip) {
        target.draw(tooltipBg);
        target.draw(tooltipName);
        target.draw(tooltipDesc);
        if (!tooltipPrice.getString().isEmpty()) target.draw(tooltipPrice);
    }
}
