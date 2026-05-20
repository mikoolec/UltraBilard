#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

void ZaladujTekstureTla(sf::Texture& tlo_stolu) // Przyjmuje referencję, nic nie kopiuje
{
    if (!tlo_stolu.loadFromFile("assets//stol_tlo.png")) {
        std::cout << "Blad: Nie znaleziono pliku stol_tlo.png!" << std::endl;
    }
    tlo_stolu.setSmooth(false); // Tutaj ustawione zadziała, bo obiekt nie zostanie skopiowany
}
void ZaladujTekstureScian(sf::Texture& sciany_stolu) // Przyjmuje referencję, nic nie kopiuje
{
    if (!sciany_stolu.loadFromFile("assets//stol_sciany.png")) {
        std::cout << "Blad: Nie znaleziono pliku stol_sciany.png!" << std::endl;
    }
    sciany_stolu.setSmooth(false); // Tutaj ustawione zadziała, bo obiekt nie zostanie skopiowany
}

class BilardBall : public sf::CircleShape
{
public:
    sf::Vector3i velocity;
    sf::IntRect bounds;
    BilardBall(float radius, const sf::Vector2f& position):
        sf::CircleShape(radius),
        velocity(sf::Vector3i(0,0,0)),
        bounds(sf::IntRect(0,0,0,0))
    {
        setPosition(position);
        setOrigin(sf::Vector2f(radius,radius));
    }

    void animate(const sf::Time &elapsed) {

        const float dt = elapsed.asSeconds();
        move(dt*velocity.x, dt*velocity.y);
        rotate(dt*velocity.z);

    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "Okno Gry");

    // Przygotowanie stylizowanego ekranu:
    sf::RenderTexture virtualScreen;
    virtualScreen.create(128,72);
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

    // Skalowanie tla na cały ekran:
    float scaleXtlo = (float)virtualScreen.getSize().x / tlo_stolu.getSize().x;
    float scaleYtlo = (float)virtualScreen.getSize().y / tlo_stolu.getSize().y;
    tlo.setScale(scaleXtlo, scaleYtlo);
    // Skalowanie scian na cały ekran:
    float scaleXsciany = (float)virtualScreen.getSize().x / sciany_stol.getSize().x;
    float scaleYsciany = (float)virtualScreen.getSize().y / sciany_stol.getSize().y;
    sciany.setScale(scaleXsciany, scaleYsciany);

    // Stworzenie kul
    vector <float> pozycjeX = {320,164,164,164,164,164};
    vector <float> pozycjeY = {180,90,270,180,135,225};
    vector<BilardBall> Kule;
    for (int i=0; i<6; i++) {
        float radi = 3;
        sf::Vector2f position(pozycjeX[i]*(128.f/640.f),pozycjeY[i]*(72.f/360.f));
        Kule.emplace_back(BilardBall(radi, position));
    }


    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

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
            if (event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    sf::Vector2f p = window.mapPixelToCoords(mouse_pos);
                    std::cout << "Mouse clicked: " << p.x << ", " << p.y << std::endl;
                }
            }

        }


        // Przygotowanie ekranu do renderowania:
        virtualScreen.clear(sf::Color::Black);
        virtualScreen.draw(tlo);
        virtualScreen.draw(sciany);


        for (auto &bal : Kule)
            virtualScreen.draw(bal);

        // Render w odpowiedniej rozdzielczości:
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
