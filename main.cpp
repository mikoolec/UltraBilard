#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

float square(float a)
{
    return a*a;
}

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
    bool Held = false;
    int identifier;
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
    virtualScreen.create(320,180);
    virtualScreen.setSmooth(false);
    // Wypróbowane wartości: 640,360 ; 320,180 ; 480,270 ; 256,144 ; 128,72

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

    // Stworzenie kul i ustawienie ich
    vector <float> pozycjebazoweX = {240,  162,162,162,162,162,  201,201,201,  220,220,  181,181,181,181,  400};
    vector <float> pozycjebazoweY = {180,  225,202,180,157,135,  157,202,180,  169,191,  146,214,169,191,  180};
    vector<BilardBall> Kule;
    for (int i=0; i<16; i++) {
        float radi = 8*(float(virtualScreen.getSize().x)/window.getSize().x);
        sf::Vector2f position(pozycjebazoweX[i]*(float(virtualScreen.getSize().x)/window.getSize().x),pozycjebazoweY[i]*(float(virtualScreen.getSize().y)/window.getSize().y));
        Kule.emplace_back(BilardBall(radi, position));
        Kule[i].identifier = i;
    }

    int lastHeldBall = -1;;
    bool isLeftPressed = false;
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
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isLeftPressed = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isLeftPressed = false;
                    lastHeldBall = -1;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                if(isLeftPressed) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    sf::Vector2f p = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());
                    std::cout << "Mouse clicked: " << p.x << ", " << p.y << std::endl;

                    for (auto &bal : Kule)
                    {
                        if( square(bal.getPosition().x - p.x) + square(bal.getPosition().y - p.y) < square(bal.getRadius()) )
                        {
                            bal.Held = true;
                            lastHeldBall = bal.identifier;
                        }
                        else
                            bal.Held = false;
                    }
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
