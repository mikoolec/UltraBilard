#include "ShopMenu.h"
#include "Globals.h"
#include <iostream>

ShopMenu::ShopMenu(std::pair<int,int> res)
{
    // Tlo i panele
    bg.setSize(sf::Vector2f(res.first, res.second));
    bg.setFillColor(sf::Color(30, 100, 40));

    leftPanel.setSize(sf::Vector2f(180, 320));
    leftPanel.setPosition(20, 20);
    leftPanel.setFillColor(sf::Color(0, 0, 0, 100));

    rightPanel.setSize(sf::Vector2f(180, 320));
    rightPanel.setPosition(res.first - 200, 20);
    rightPanel.setFillColor(sf::Color(0, 0, 0, 100));

    // Srodek
    if(font.loadFromFile("assets//PublicPixel.ttf")) {
        titleText.setFont(font); titleText.setString("SKLEP");
        titleText.setCharacterSize(30); titleText.setFillColor(sf::Color::White);
        sf::FloatRect tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width/2.0f, tb.top + tb.height/2.0f);
        titleText.setPosition(res.first/2.0f, 40);

        tooltipName.setFont(font); tooltipName.setCharacterSize(12); tooltipName.setFillColor(sf::Color::Yellow);
        tooltipDesc.setFont(font); tooltipDesc.setCharacterSize(10); tooltipDesc.setFillColor(sf::Color::White);
        tooltipPrice.setFont(font); tooltipPrice.setCharacterSize(14); tooltipPrice.setFillColor(sf::Color::Green);
    }

    // Przyciski kijow
    std::vector<std::string> cueNames = {"1", "2", "3"};
    std::vector<std::string> cueDescs = {"a", "b", "c"};
    int startY = 80;

    for(int i = 0; i < 3; i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(140, 50));
        btn.shape.setOrigin(70, 25);
        btn.shape.setPosition(110, startY + (i * 70));
        btn.shape.setFillColor(sf::Color(150, 150, 150)); // Szary przycisk

        btn.nazwa = cueNames[i];
        btn.opis = cueDescs[i];
        btn.cena = 50 + (i * 25); // Ceny

        cueButtons.push_back(btn);
    }

    // Przyciski bil
    std::vector<std::string> ballNames = {"1", "2", "3", "4", "5", "6"};
    std::vector<std::string> ballDescs = {"a", "b", "c", "d", "e", "f"};

    for(int i = 0; i < 6; i++)
    {
        ShopButton btn;
        btn.shape.setSize(sf::Vector2f(60, 60)); // Kwadraty
        btn.shape.setOrigin(30, 30);

        // Układanie w siatce (2 kolumny, 3 wiersze)
        int col = i % 2;
        int row = i / 2;
        btn.shape.setPosition(490 + (col * 80), 100 + (row * 80));
        btn.shape.setFillColor(sf::Color(150, 150, 150));

        btn.nazwa = ballNames[i];
        btn.opis = ballDescs[i];
        btn.cena = 30 + (i * 10);

        ballButtons.push_back(btn);
    }

    // Przycisk next
    btnNextShape.setSize(sf::Vector2f(160, 40));
    btnNextShape.setOrigin(80, 20);
    btnNextShape.setPosition(res.first/2.0f, res.second - 40);
    btnNextShape.setFillColor(sf::Color(200, 50, 50)); // Czerwony powrót

    btnNextText.setFont(font); btnNextText.setString("NASTEPNA RUNDA");
    btnNextText.setCharacterSize(10); btnNextText.setFillColor(sf::Color::White);
    sf::FloatRect nb = btnNextText.getLocalBounds();
    btnNextText.setOrigin(nb.left + nb.width/2.0f, nb.top + nb.height/2.0f);
    btnNextText.setPosition(res.first/2.0f, res.second - 40);
}

void ShopMenu::updateHover(sf::Vector2f mousePos)
{
    hoveredItem = nullptr; // Resetujemy hovered item co klatkę

    // Sprawdzamy Kije
    for(auto& btn : cueButtons)
    {
        if(btn.shape.getGlobalBounds().contains(mousePos))
        {
            btn.shape.setFillColor(sf::Color(200, 200, 200)); // Podświetlenie
            hoveredItem = &btn;
        } else
        {
            btn.shape.setFillColor(sf::Color(150, 150, 150)); // Powrót do szarego
        }
    }

    // Sprawdzamy Bile
    for(auto& btn : ballButtons)
    {
        if(btn.shape.getGlobalBounds().contains(mousePos))
        {
            btn.shape.setFillColor(sf::Color(200, 200, 200));
            hoveredItem = &btn;
        } else
        {
            btn.shape.setFillColor(sf::Color(150, 150, 150));
        }
    }

    // Przycisk Next
    if(btnNextShape.getGlobalBounds().contains(mousePos)) btnNextShape.setFillColor(sf::Color(255, 100, 100));
    else btnNextShape.setFillColor(sf::Color(200, 50, 50));

    // Aktualizacja Tekstów na środku jeśli coś jest najechane
    if(hoveredItem)
    {
        tooltipName.setString(hoveredItem->nazwa);
        tooltipDesc.setString(hoveredItem->opis);
        tooltipPrice.setString("Cena: " + std::to_string(hoveredItem->cena) + " Monet");

        sf::FloatRect nb = tooltipName.getLocalBounds();
        tooltipName.setOrigin(nb.left + nb.width/2.0f, nb.top + nb.height/2.0f);
        tooltipName.setPosition(320, 140);

        sf::FloatRect db = tooltipDesc.getLocalBounds();
        tooltipDesc.setOrigin(db.left + db.width/2.0f, db.top + db.height/2.0f);
        tooltipDesc.setPosition(320, 170);

        sf::FloatRect pb = tooltipPrice.getLocalBounds();
        tooltipPrice.setOrigin(pb.left + pb.width/2.0f, pb.top + pb.height/2.0f);
        tooltipPrice.setPosition(320, 210);
    }
}

int ShopMenu::handleClick(sf::Vector2f mousePos)
{
    if (btnNextShape.getGlobalBounds().contains(mousePos)) return 6; // Next Round

    // TODO zakup
    if (hoveredItem)
    {
        std::cout << "Sklep - Kliknieto: " << hoveredItem->nazwa << " za " << hoveredItem->cena << " monet!" << std::endl;
        // TODO monety itd
    }

    return 0;
}

void ShopMenu::draw(sf::RenderTexture& target)
{
    target.draw(bg);
    target.draw(leftPanel);
    target.draw(rightPanel);

    target.draw(titleText);

    for(auto& btn : cueButtons) target.draw(btn.shape);
    for(auto& btn : ballButtons) target.draw(btn.shape);

    target.draw(btnNextShape);
    target.draw(btnNextText);

    // Rysujemy środkowe opisy tylko, jeśli najeżdżamy myszką na jakiś przedmiot
    if(hoveredItem) {
        target.draw(tooltipName);
        target.draw(tooltipDesc);
        target.draw(tooltipPrice);
    }
}