#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

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

float square(float a)
{
    return a*a;
}

sf::Vector2f normal(sf::Vector2f V)
{
    float len;
    len = square(V.x) + square(V.y);
    len = sqrt(len);
    if(len==0)
    {
        return sf::Vector2f(0,0);
    }
    V = V/len;
    return V;
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

class BilardHole : public sf::CircleShape
{
public:
    int identifier;
    sf::IntRect bounds;
    BilardHole(float radius, const sf::Vector2f& position):
        sf::CircleShape(radius),
        bounds(sf::IntRect(0,0,0,0))
    {
        setPosition(position);
        setOrigin(sf::Vector2f(radius,radius));
        setFillColor(sf::Color(0,0,0));
    }
};

class BilardBall : public sf::CircleShape
{
public:
    bool Held = false;
    int identifier;
    float tarcie;
    float tarciescian;
    sf::Vector2f velocity;
    int rotation;
    sf::IntRect bounds;
    BilardBall(float radius, const sf::Vector2f& position):
        sf::CircleShape(radius),
        velocity(sf::Vector2f(0,0)),
        bounds(sf::IntRect(0,0,0,0))
    {
        setPosition(position);
        setOrigin(sf::Vector2f(radius,radius));
        rotation = 0;
        tarcie = 0.125f;
        tarciescian = 5.f;
    }

    void rob_tarcie(float sila, bool sciana)
    {
        if(sciana)
        {
            sf::Vector2f velc_po_tarc;
            velc_po_tarc = sf::Vector2f( this->velocity - ( normal(this->velocity) * this->tarciescian * sila ) );
            if( this->velocity.x * velc_po_tarc.x < 0 ) velc_po_tarc.x = 0.f;
            if( this->velocity.y * velc_po_tarc.y < 0 ) velc_po_tarc.y = 0.f;
            this->setVelocity(velc_po_tarc);
        }
        else
        {
            sf::Vector2f velc_po_tarc;
            velc_po_tarc = sf::Vector2f( this->velocity - ( normal(this->velocity) * this->tarcie * sila ) );
            if( this->velocity.x * velc_po_tarc.x < 0 ) velc_po_tarc.x = 0.f;
            if( this->velocity.y * velc_po_tarc.y < 0 ) velc_po_tarc.y = 0.f;
            this->setVelocity(velc_po_tarc);
        }

    }

    void animate(const sf::Time &elapsed, std::vector<BilardBall> &Kule, const std::vector<BilardHole> &Dziury, float tarcieScianGlobal, float tarcieStoluGlobal) {

        const float dt = elapsed.asSeconds();
        move(dt*velocity.x, dt*velocity.y);
        rotate(dt*rotation);

        // Spowalnianie przez tarcie
        this->rob_tarcie(tarcieStoluGlobal, 0);

        // Sprawdzenie kolizji ścian
        if(this->getPosition().x + this->getRadius() > 620)
        {
            this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
            this->move(sf::Vector2f(620.f - this->getPosition().x - this->getRadius() - 2, 0));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().x - this->getRadius() < 20)
        {
            this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
            this->move(sf::Vector2f(20.f - this->getPosition().x + this->getRadius() + 2, 0));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().y + this->getRadius() > 334)
        {
            this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
            this->move(sf::Vector2f(0, 334.f - this->getPosition().y - this->getRadius() - 2));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().y - this->getRadius() < 24)
        {
            this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
            this->move(sf::Vector2f(0, 24.f - this->getPosition().y + this->getRadius() + 2));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }


    }

    void setVelocity(sf::Vector2f vel)
    {
        velocity = vel;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "Okno Gry");

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

    // Skalowanie tla na cały ekran:
    float scaleXtlo = (float)virtualScreen.getSize().x / tlo_stolu.getSize().x;
    float scaleYtlo = (float)virtualScreen.getSize().y / tlo_stolu.getSize().y;
    tlo.setScale(scaleXtlo, scaleYtlo);
    // Skalowanie scian na cały ekran:
    float scaleXsciany = (float)virtualScreen.getSize().x / sciany_stol.getSize().x;
    float scaleYsciany = (float)virtualScreen.getSize().y / sciany_stol.getSize().y;
    sciany.setScale(scaleXsciany, scaleYsciany);

    // Stworzenie dziur
    vector <float> pozycjedziurX = {320, 320, 24 ,26 , 614 ,614 };
    vector <float> pozycjedziurY = {23,  330, 27 ,328, 26, 330 };
    vector <BilardHole> Dziury;
    for (int i=0; i<6; i++)
    {
        float radi = 20*(float(virtualScreen.getSize().x)/window.getSize().x);
        sf::Vector2f position(pozycjedziurX[i]*(float(virtualScreen.getSize().x)/window.getSize().x),pozycjedziurY[i]*(float(virtualScreen.getSize().y)/window.getSize().y));
        Dziury.emplace_back(BilardHole(radi,position));
    }

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


    // Zmienne do ulepszeń
    float silaStrzalu = 1;
    float tarcieStoluGlobal = 1;
    float tarcieScianGlobal = 1;
    bool widocznoscCelu = true; // do testów, w grze zmienić na false żeby można było kupić

    // Zmienne do działania
    int lastHeldBall = -1;;
    bool isLeftPressed = false;
    bool roundIsActive = false;
    bool isDragging = false;
    bool accelerateWhiteNow = false;
    float velc = 0.f;
    float dist_cent;
    sf::Vector2f addVelocity;
    sf::Vector2f p;
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

                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    p = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());
                    sf::Vector2f pp = window.mapPixelToCoords(mouse_pos);
                    std::cout << "Mouse clicked: " << pp.x << ", " << pp.y << std::endl;
                    std::cout << "Mouse clicked retro: " << p.x << ", " << p.y << std::endl;

                    isLeftPressed = true;
                    // Tymczasowe włączanie gry
                    roundIsActive = true;

                    for (auto &bal : Kule)
                    {
                        if( square(bal.getPosition().x - p.x) + square(bal.getPosition().y - p.y) < square(bal.getRadius()) )
                        {
                            bal.Held = true;
                            if(bal.identifier == 15)
                            {
                                lastHeldBall = bal.identifier;
                            }

                        }
                        else
                        {
                            bal.Held = false;
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isLeftPressed = false;
                    lastHeldBall = -1;
                    if(isDragging)
                    {
                        accelerateWhiteNow = true;
                    }
                    isDragging = false;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                if(isLeftPressed) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    p = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());

                    for (auto &bal : Kule)
                    {
                        float dist_cent = square(bal.getPosition().x - p.x) + square(bal.getPosition().y - p.y);
                        if( dist_cent < square(bal.getRadius()) )
                        {
                            bal.Held = true;
                            if(bal.identifier == 15)
                            {
                                lastHeldBall = bal.identifier;
                            }
                        }
                        else
                        {
                            bal.Held = false;
                        }
                    }
                }
            }

        }

        // Sprawdzanie czy celujemy
        dist_cent = square(Kule[15].getPosition().x - p.x) + square(Kule[15].getPosition().y - p.y);
        dist_cent = sqrt(dist_cent);
        if (lastHeldBall == 15 && dist_cent > Kule[15].getRadius() + 4 )
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
            cout<<"velc = "<<velc<<endl;
            addVelocity = sf::Vector2f(0,0);
            addVelocity.x = (8*velc/dist_cent)*(Kule[15].getPosition().x-p.x) ;
            addVelocity.y = (8*velc/dist_cent)*(Kule[15].getPosition().y-p.y) ;
            Kule[15].setVelocity(addVelocity * silaStrzalu);
            velc = 0;
        }


        // Przemieszczenie kul
        for (auto &bal : Kule)
        {
            bal.animate(elapsed, Kule, Dziury, tarcieScianGlobal, tarcieStoluGlobal);
        }

        // Przygotowanie ekranu do renderowania:
        virtualScreen.clear(sf::Color::Black);
        virtualScreen.draw(tlo);
        for (auto &hol : Dziury)
            virtualScreen.draw(hol);
        virtualScreen.draw(sciany);

        // Render
        if(roundIsActive)
        {
            for (auto &bal : Kule)
                virtualScreen.draw(bal);

            // Celownik
            if(isDragging)
            {
                if(widocznoscCelu)
                {
                    sf::Vector2f plin;
                    plin.x = Kule[15].getPosition().x + (2*velc/dist_cent)*(Kule[15].getPosition().x-p.x) ;
                    plin.y = Kule[15].getPosition().y + (2*velc/dist_cent)*(Kule[15].getPosition().y-p.y) ;
                    RysujLinie(virtualScreen, Kule[15].getPosition(), plin, 2.f, sf::Color::White);
                }
            }
        }

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
