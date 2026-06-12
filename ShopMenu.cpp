#include "ShopMenu.h"
#include "Globals.h"
#include "Upgrades.h"
#include <iostream>
#include<algorithm>
#include<random>

ShopMenu::ShopMenu(std::pair<int,int> res) {
    // Tła i panele
    bg.setSize(sf::Vector2f(res.first, res.second));
    bg.setFillColor(sf::Color(30, 100, 40));

    leftPanel.setSize(sf::Vector2f(180, 320));
    leftPanel.setPosition(20, 20);
    leftPanel.setFillColor(sf::Color(0, 0, 0, 100));

    rightPanel.setSize(sf::Vector2f(180, 320));
    rightPanel.setPosition(res.first - 200, 20);
    rightPanel.setFillColor(sf::Color(0, 0, 0, 100));

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

        // Ustawienia pływającego okienka
        tooltipName.setFont(font); tooltipName.setCharacterSize(12); tooltipName.setFillColor(sf::Color::Yellow);
        tooltipDesc.setFont(font); tooltipDesc.setCharacterSize(10); tooltipDesc.setFillColor(sf::Color::White);
        tooltipPrice.setFont(font); tooltipPrice.setCharacterSize(12); tooltipPrice.setFillColor(sf::Color::Green);
    }

    // Pływające okienko tło
    tooltipBg.setFillColor(sf::Color(0, 0, 0, 220));
    tooltipBg.setOutlineThickness(2);
    tooltipBg.setOutlineColor(sf::Color::White);

    // Ekwipunek
    currentCueDisplay.setSize(sf::Vector2f(140, 20));
    currentCueDisplay.setOrigin(70, 10);
    currentCueDisplay.setPosition(res.first/2.0f, 140);
    currentCueDisplay.setFillColor(sf::Color(139, 69, 19));

    currentTriangleDisplay.setPointCount(3);
    currentTriangleDisplay.setPoint(0, sf::Vector2f(0, -30));
    currentTriangleDisplay.setPoint(1, sf::Vector2f(30, 30));
    currentTriangleDisplay.setPoint(2, sf::Vector2f(-30, 30));
    currentTriangleDisplay.setFillColor(sf::Color(100, 100, 100));
    currentTriangleDisplay.setPosition(res.first/2.0f, 220);

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

    // przyciski sklep
    // przyciski kije
    int startY = 110;
    for(int i = 0; i < wylosowaneKije.size(); i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(140, 50));
        btn.shape.setOrigin(70, 25);
        btn.shape.setPosition(110, startY + (i * 70));
        btn.shape.setFillColor(sf::Color(150, 150, 150));

        // dane z losowych kiji
        btn.nazwa = wylosowaneKije[i].nazwa;
        btn.opis = wylosowaneKije[i].opis;
        btn.cena = wylosowaneKije[i].cena;

        cueButtons.push_back(btn);
    }

    // przyciski bile
    for(int i = 0; i < wylosowaneBile.size(); i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(60, 60));
        btn.shape.setOrigin(30, 30);
        int col = i % 2;
        int row = i / 2;
        btn.shape.setPosition(490 + (col * 80), 140 + (row * 80));
        btn.shape.setFillColor(sf::Color(150, 150, 150));

        // dane z losowych bil
        btn.nazwa = wylosowaneBile[i].nazwa;
        btn.opis = wylosowaneBile[i].opis;
        btn.cena = wylosowaneBile[i].cena;

        ballButtons.push_back(btn);
    }

    // Przycisk next
    btnNextShape.setSize(sf::Vector2f(160, 40)); btnNextShape.setOrigin(80, 20);
    btnNextShape.setPosition(res.first/2.0f, res.second - 40); btnNextShape.setFillColor(sf::Color(200, 50, 50));
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
            ballCount++;
        }
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
                if(!btn.kupiony) btn.shape.setFillColor(sf::Color(200, 200, 200));
                hoveredItem = &btn;
                tooltipName.setString(btn.nazwa);
                tooltipDesc.setString(btn.opis);
                tooltipPrice.setString("Cena: " + std::to_string(btn.cena));
                showTooltip = true;
            }
            else
            {
                if(btn.kupiony) btn.shape.setFillColor(sf::Color(50, 150, 50));
                else {btn.shape.setFillColor(sf::Color(150, 150, 150));}
            }
        }

        // Sprawdzamy Bile do kupienia
        for(auto& btn : ballButtons)
        {
            if(btn.shape.getGlobalBounds().contains(mousePos))
            {
                if(!btn.kupiony) btn.shape.setFillColor(sf::Color(200, 200, 200));
                hoveredItem = &btn;
                tooltipName.setString(btn.nazwa);
                tooltipDesc.setString(btn.opis);
                tooltipPrice.setString("Cena: " + std::to_string(btn.cena));
                showTooltip = true;
            }
            else
            {
                if(btn.kupiony) btn.shape.setFillColor(sf::Color(50, 150, 50));
                else {btn.shape.setFillColor(sf::Color(150, 150, 150));}
            }
        }

        // Sprawdzanie ikony trójkąta na środku
        if(currentTriangleDisplay.getGlobalBounds().contains(mousePos))
        {
            currentTriangleDisplay.setFillColor(sf::Color(150, 150, 150));
            tooltipName.setString("Twoje Bile"); tooltipDesc.setString("Kliknij, aby sprawdzic"); tooltipPrice.setString("");
            showTooltip = true;
        } else
        {
            currentTriangleDisplay.setFillColor(sf::Color(100, 100, 100));
        }

        // Sprawdzanie obecnego kija na środku
        if(currentCueDisplay.getGlobalBounds().contains(mousePos))
        {
            tooltipName.setString("Zwykly Kij"); tooltipDesc.setString("Twoj obecny sprzet"); tooltipPrice.setString("");
            showTooltip = true;
        }

        if(btnNextShape.getGlobalBounds().contains(mousePos)) btnNextShape.setFillColor(sf::Color(255, 100, 100));
        else btnNextShape.setFillColor(sf::Color(200, 50, 50));

    }
    else if (currentSubState == SHOP_BALL_INVENTORY)
    {
        // Powiększonym widok trójkąta
        for (int i = 0; i < inventoryBalls.size(); i++)
        {
            if (inventoryBalls[i].getGlobalBounds().contains(mousePos))
            {
                tooltipName.setString("Bila " + std::to_string(i+1));
                tooltipDesc.setString("Obecnie brak ulepszen");
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
        float height = tooltipPrice.getString().isEmpty() ? 45 : 65;

        // Zabezpieczenie przed uciekaniem w prawo
        if (tooltipX + maxWidth + 10 > 640)
        {
            tooltipX = mousePos.x - maxWidth - 25;
        }

        // Zabezpieczenie przed uciekaniem w dół pod ekran
        if (tooltipY + height + 10 > 360)
        {
            tooltipY = mousePos.y - height - 15;
        }

        tooltipName.setPosition(tooltipX + 5, tooltipY + 5);
        tooltipDesc.setPosition(tooltipX + 5, tooltipY + 25);
        tooltipPrice.setPosition(tooltipX + 5, tooltipY + 45);

        tooltipBg.setPosition(tooltipX, tooltipY);
        tooltipBg.setSize(sf::Vector2f(maxWidth + 10, height));
    }
}

int ShopMenu::handleClick(sf::Vector2f mousePos)
{
    if (currentSubState == SHOP_MAIN)
    {
        // Przejście do widoku szczegółów bil
        if (currentTriangleDisplay.getGlobalBounds().contains(mousePos))
        {
            currentSubState = SHOP_BALL_INVENTORY;
            return 0; // Nie wychodzimy ze sklepu, zmieniamy tylko stan
        }

        if (btnNextShape.getGlobalBounds().contains(mousePos)) return 6; // Next Round

        if (hoveredItem != nullptr) {
            if (hoveredItem->kupiony) {
                std::cout << "Sklep - Posiadasz juz ten przedmiot" << std::endl;
            }
            else if (g_Stats.kupUlepszenie(hoveredItem->cena))
            {
                std::cout << "Sklep - Kupiono: " << hoveredItem->nazwa << "! Pozostalo monet: " << g_Stats.monety << std::endl;
                hoveredItem->kupiony = true;
                g_Stats.dodajUpgrade(hoveredItem->id); // dodajemy id do inventory
                hoveredItem->shape.setFillColor(sf::Color(50, 150, 50));
                // TODO check itemow konkretnych
            }
            else
            {
                std::cout << "Sklep - Za malo monet!" << std::endl;
            }
        }
    }
    else if (currentSubState == SHOP_BALL_INVENTORY) {
        // Zamknięcie pod-menu bil po kliknięciu gdziekolwiek indziej niż w bile (albo po prostu gdziekolwiek)
        currentSubState = SHOP_MAIN;
    }

    return 0;
}

void ShopMenu::draw(sf::RenderTexture& target) {
    target.draw(bg);

    if (currentSubState == SHOP_MAIN) {
        target.draw(leftPanel); target.draw(rightPanel);
        target.draw(titleText); target.draw(inventoryText);

        // Rysowanie boxów kupowania
        for(auto& btn : cueButtons) target.draw(btn.shape);
        for(auto& btn : ballButtons) target.draw(btn.shape);

        // Rysowanie środka
        target.draw(currentCueDisplay);
        target.draw(currentTriangleDisplay);

        target.draw(btnNextShape); target.draw(btnNextText);
    }
    else if (currentSubState == SHOP_BALL_INVENTORY) {
        // Widok inwentarza bil
        target.draw(leftPanel); target.draw(rightPanel);
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

