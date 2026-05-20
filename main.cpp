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
    virtualScreen.create(640, 360);
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
        }


        // Przygotowanie ekranu do renderowania:
        virtualScreen.clear(sf::Color::Black);
        virtualScreen.draw(tlo);
        virtualScreen.draw(sciany);



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
