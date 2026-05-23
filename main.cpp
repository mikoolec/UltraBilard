#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

enum GameState
{
    MENU,
    PLAYING,
    SHOP,
    GAME_OVER
};

enum MenuAction
{
    ActionNone,
    ActionPlay,
    ActionSettings,
    ActionQuit
};

enum GameOverAction
{
    GO_ActionNone,
    GO_ActionRetry,
    GO_ActionMenu
};

struct GameStats
{
    int punktyTejRundy = 0;
    int punktyGlobalnie = 0;
    int monety = 0;
    int monetyGlobalnie = 0;
    int rundy = 1;
    int wbiteBileGlobalnie = 0;
    int strzalyGlobalnie = 0;
}g_Stats;

struct AudioManager
{
    sf::SoundBuffer wallhitBuffer;
    sf::SoundBuffer ballhitBuffer;


    void init()
    {
        // Importowanie sfx:
        if (!wallhitBuffer.loadFromFile("assets//sound_wall.wav")) {
            std::cout << "Blad: Nie znaleziono assets/sound_wall.wav!" << std::endl;
        }
        if (!ballhitBuffer.loadFromFile("assets//sound_ball.wav")) {
            std::cout << "Blad: Nie znaleziono assets/sound_ball.wav!" << std::endl;
        }
    }
}g_Audio;

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

float diff(sf::Vector2f a, sf::Vector2f b)
{
    float d;
    d = square( a.x - b.x ) + square( a.y - b.y );
    d = sqrt(d);
    return d;
}

float diff(sf::Vector2f a)
{
    float d;
    d = square( a.x ) + square( a.y );
    d = sqrt(d);
    return d;
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

class MainMenu
{
private:
    sf::Texture textStartNormal, textStartHover;
    sf::Sprite btnStart;
    sf::Texture textSettingsNormal, textSettingsHover;
    sf::Sprite btnSettings;
    sf::Texture textQuitNormal, textQuitHover;
    sf::Sprite btnQuit;
    sf::Font font;
    sf::Text titleText;

    void setupButton(sf::Sprite& btn, sf::Texture& textNorm, sf::Texture& textHov,std::string pathNorm, std::string pathHov, float x, float y)
    {
        if(!textNorm.loadFromFile(pathNorm) || !textHov.loadFromFile(pathHov))
        {
            std::cout<<"Brak grafik menu"<<std::endl;
        }
        btn.setTexture(textNorm);
        sf::FloatRect bounds = btn.getLocalBounds();
        btn.setOrigin(bounds.width/2.0f,bounds.height/2.0f);
        btn.setPosition(x,y);
    }
public:
    MainMenu(std::pair<int,int> res)
    {
        if(font.loadFromFile("assets//PublicPixel.ttf"))
        {
            titleText.setFont(font);
            titleText.setString("UltraBilard");
            titleText.setCharacterSize(50);
            titleText.setFillColor(sf::Color::White);
            sf::FloatRect titleBounds = titleText.getLocalBounds();
            titleText.setOrigin(titleBounds.left + titleBounds.width /2.0f, titleBounds.top + titleBounds.height /2.0f);
            titleText.setPosition(res.first/2.0f,60);
        }
        setupButton(btnStart,textStartNormal,textStartHover, "assets//przycisk_start_normal.png","assets//przycisk_start_hover.png",res.first/2.0f,160);
        setupButton(btnSettings,textSettingsNormal,textSettingsHover, "assets//przycisk_settings_normal.png","assets//przycisk_settings_hover.png",res.first/2.0f,230);
        setupButton(btnQuit,textQuitNormal,textQuitHover, "assets//przycisk_quit_normal.png","assets//przycisk_quit_hover.png",res.first/2.0f,300);
    }

    void updateHover(sf::Vector2f mousePos)
    {
        // start
        if(btnStart.getGlobalBounds().contains(mousePos))
        {
            btnStart.setTexture(textStartHover);
            btnStart.setScale(1.1f,1.1f);
        }
        else
        {
            btnStart.setTexture(textStartNormal);
            btnStart.setScale(1.0f,1.0f);
        }
        // settings
        if(btnSettings.getGlobalBounds().contains(mousePos))
        {
            btnSettings.setTexture(textSettingsHover);
            btnSettings.setScale(1.1f,1.1f);
        }
        else
        {
            btnSettings.setTexture(textSettingsNormal);
            btnSettings.setScale(1.0f,1.0f);
        }
        // quit
        if(btnQuit.getGlobalBounds().contains(mousePos))
        {
            btnQuit.setTexture(textQuitHover);
            btnQuit.setScale(1.1f,1.1f);
        }
        else
        {
            btnQuit.setTexture(textQuitNormal);
            btnQuit.setScale(1.0f,1.0f);
        }
    }

    MenuAction handleClick(sf::Vector2f mousePos)
    {
        if(btnStart.getGlobalBounds().contains(mousePos)) return ActionPlay;
        if(btnSettings.getGlobalBounds().contains(mousePos)) return ActionSettings;
        if(btnQuit.getGlobalBounds().contains(mousePos)) return ActionQuit;
        return ActionNone;
    }

    void draw(sf::RenderTexture& target)
    {
        target.draw(titleText);
        target.draw(btnStart);
        target.draw(btnSettings);
        target.draw(btnQuit);
    }
};


class GameOverScreen
{
private:
    sf::RectangleShape bgFilter;
    sf::Texture texPanel,texTitle,texLine,texCoinIcon;
    sf::Texture texLabelScore, texLabelCoins, texLabelRounds, texLabelBiles, texLabelShots;
    sf::Texture texBtnRetryNorm, texBtnRetryHov, texBtnMenuNorm, texBtnMenuHov;

    sf::Sprite sprPanel,sprTitle,sprLine,sprCoinIcon;
    sf::Sprite sprLabelScore, sprLabelCoins, sprLabelRounds, sprLabelBiles, sprLabelShots;
    sf::Sprite sprBtnRetry, sprBtnMenu;

    sf::Font font;
    sf::Text valScore, valCoins, valRounds, valBiles, valShots;

void loadAndSetup(sf::Texture& tex, sf::Sprite& spr, std::string path)
    {
        if(!tex.loadFromFile(path))
        {
            std::cout<<"brak"<<path<<std::endl;
        }
        tex.setSmooth(false);
        spr.setTexture(tex);
    }
public:
    GameOverScreen(std::pair<int,int> res)
    {
        bgFilter.setSize(sf::Vector2f(res.first,res.second));
        bgFilter.setFillColor(sf::Color(0,0,0,200));

        loadAndSetup(texPanel, sprPanel, "assets//GO_tlo.png");
        loadAndSetup(texTitle, sprTitle, "assets//GO_GO.png");
        loadAndSetup(texLine, sprLine, "assets//GO_prosta.png");
        loadAndSetup(texCoinIcon, sprCoinIcon, "assets//monetka.png");

        loadAndSetup(texLabelScore, sprLabelScore, "assets//GO_zdobyte_punkty.png");
        loadAndSetup(texLabelCoins, sprLabelCoins, "assets//GO_zebrane_monety.png");
        loadAndSetup(texLabelRounds, sprLabelRounds, "assets//GO_rundy.png");
        loadAndSetup(texLabelBiles, sprLabelBiles, "assets//GO_wbite_bile.png");
        loadAndSetup(texLabelShots, sprLabelShots, "assets//GO_ilosc_strzalow.png");

        if (!texBtnRetryNorm.loadFromFile("assets//GO_new_run_normal.png") ||
            !texBtnRetryHov.loadFromFile("assets//GO_new_run_hover.png") ||
            !texBtnMenuNorm.loadFromFile("assets//GO_main_menu_normal.png") ||
            !texBtnMenuHov.loadFromFile("assets//GO_main_menu_hover.png")) {
            std::cout << "brak grafik game over" << std::endl;
        }

        texBtnRetryNorm.setSmooth(false); texBtnRetryHov.setSmooth(false);
        texBtnMenuNorm.setSmooth(false); texBtnMenuHov.setSmooth(false);

        sf::Vector2f center(res.first/2.0f,res.second/2.0f);

        sf::FloatRect pb = sprPanel.getLocalBounds();
        sprPanel.setOrigin(pb.width / 2.0f, pb.height / 2.0f);
        sprPanel.setPosition(center);

        sf::FloatRect tb = sprTitle.getLocalBounds();
        sprTitle.setOrigin(tb.width / 2.0f, tb.height / 2.0f);
        sprTitle.setPosition(center.x, center.y - 130);
        sprTitle.setScale(1.8f,1.8f);

        sf::FloatRect lb = sprLine.getLocalBounds();
        sprLine.setOrigin(lb.width / 2.0f, lb.height / 2.0f);
        sprLine.setPosition(center.x, center.y - 110);

        float statsLeftX = center.x - 130; // punkt startowy napisów
        float statsValueX = center.x + 40; // punkt startowy liczb

        sprLabelScore.setPosition(statsLeftX, center.y - 80);
        sprLabelCoins.setPosition(statsLeftX, center.y - 50);
        sprLabelRounds.setPosition(statsLeftX, center.y - 20);
        sprLabelBiles.setPosition(statsLeftX, center.y + 10);
        sprLabelShots.setPosition(statsLeftX, center.y + 40);

        sprBtnRetry.setTexture(texBtnRetryNorm);
        sf::FloatRect rb = sprBtnRetry.getLocalBounds();
        sprBtnRetry.setOrigin(rb.width / 2.0f, rb.height / 2.0f);
        sprBtnRetry.setPosition(center.x, center.y + 90);
        sprBtnRetry.setScale(0.6f,0.6f);

        sprBtnMenu.setTexture(texBtnMenuNorm);
        sf::FloatRect mb = sprBtnMenu.getLocalBounds();
        sprBtnMenu.setOrigin(mb.width / 2.0f, mb.height / 2.0f);
        sprBtnMenu.setPosition(center.x, center.y + 130);
        sprBtnMenu.setScale(0.6f,0.6f);

        if (font.loadFromFile("assets//PublicPixel.ttf"))
        {
            valScore.setFont(font); valScore.setCharacterSize(16); valScore.setFillColor(sf::Color::White);
            valCoins.setFont(font); valCoins.setCharacterSize(16); valCoins.setFillColor(sf::Color::White);
            valRounds.setFont(font); valRounds.setCharacterSize(16); valRounds.setFillColor(sf::Color::White);
            valBiles.setFont(font); valBiles.setCharacterSize(16); valRounds.setFillColor(sf::Color::White);
            valShots.setFont(font); valShots.setCharacterSize(16); valRounds.setFillColor(sf::Color::White);

            // pozycje liczb
            valScore.setPosition(statsValueX, sprLabelScore.getPosition().y - 3);
            valCoins.setPosition(statsValueX, sprLabelCoins.getPosition().y - 3);
            valRounds.setPosition(statsValueX, sprLabelRounds.getPosition().y - 3);
            valBiles.setPosition(statsValueX, sprLabelBiles.getPosition().y - 3);
            valShots.setPosition(statsValueX, sprLabelShots.getPosition().y - 3);
        }
    }

    void updateStats(const GameStats& stats)
    {
        valScore.setString(std::to_string(stats.punktyGlobalnie));
        valCoins.setString(std::to_string(stats.monetyGlobalnie));
        valRounds.setString(std::to_string(stats.rundy));
        valBiles.setString(std::to_string(stats.wbiteBileGlobalnie));
        valShots.setString(std::to_string(stats.strzalyGlobalnie));

        float rightEdgeX=sprPanel.getPosition().x+120.0f;

        auto alignTextRight = [&](sf::Text& text)
        {
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left+bounds.width,0);
            text.setPosition(rightEdgeX,text.getPosition().y);
        };
        alignTextRight(valScore);
        alignTextRight(valCoins);
        alignTextRight(valRounds);
        alignTextRight(valBiles);
        alignTextRight(valShots);

        sprCoinIcon.setPosition(rightEdgeX+10.0f,valCoins.getPosition().y+2.0f);
    }

    void updateHover(sf::Vector2f mousePos)
    {
        if (sprBtnRetry.getGlobalBounds().contains(mousePos)) sprBtnRetry.setTexture(texBtnRetryHov);
        else sprBtnRetry.setTexture(texBtnRetryNorm);

        if (sprBtnMenu.getGlobalBounds().contains(mousePos)) sprBtnMenu.setTexture(texBtnMenuHov);
        else sprBtnMenu.setTexture(texBtnMenuNorm);
    }

    GameOverAction handleClick(sf::Vector2f mousePos)
    {
        if (sprBtnRetry.getGlobalBounds().contains(mousePos)) return GO_ActionRetry;
        if (sprBtnMenu.getGlobalBounds().contains(mousePos)) return GO_ActionMenu;
        return GO_ActionNone;
    }

    void draw(sf::RenderTexture& target)
    {
        target.draw(bgFilter);
        target.draw(sprPanel);
        target.draw(sprTitle);
        target.draw(sprLine);

        target.draw(sprLabelScore);
        target.draw(sprLabelCoins);
        target.draw(sprCoinIcon);
        target.draw(sprLabelRounds);
        target.draw(sprLabelBiles);
        target.draw(sprLabelShots);

        target.draw(valScore);
        target.draw(valCoins);
        target.draw(valRounds);
        target.draw(valBiles);
        target.draw(valShots);

        target.draw(sprBtnRetry);
        target.draw(sprBtnMenu);
    }
};


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

    // Zmienne do ulepszeń
    float tarcie;
    float tarciescian;
    int wartoscPunktowa;
    float mnoznikPunktowy;
    float radiusMultipl;
    int punktyNaUderzeniuKuli;
    int punktyNaUderzeniuSciany;
    // Zmienne do działania
    bool Held = false;
    bool Put = false;
    int identifier;
    vector <int> saveBounces;
    sf::Vector2f velocity;
    int rotation;
    sf::IntRect bounds;

    sf::Sound wallhitSFX;
    sf::Sound ballhitSFX;

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
        wartoscPunktowa = 1;
        mnoznikPunktowy = 1;
        radiusMultipl = 1;
        punktyNaUderzeniuKuli = 0;
        punktyNaUderzeniuSciany = 0;

        wallhitSFX.setBuffer(g_Audio.wallhitBuffer);
        wallhitSFX.setVolume(20.f);
        ballhitSFX.setBuffer(g_Audio.ballhitBuffer);
        ballhitSFX.setVolume(15.f);
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

    void ballPut()
    {
        if(this->identifier != 15)
        {
            g_Stats.punktyTejRundy += this->wartoscPunktowa;
            g_Stats.punktyTejRundy *= this->mnoznikPunktowy;
            g_Stats.monety+=99999; // ilosc do ustalenia
            //statystyki do GO
            g_Stats.monetyGlobalnie+=g_Stats.monety; // ilosc do ustalenia
            g_Stats.wbiteBileGlobalnie+=1;
            //
            this->Put = true;
            cout<<"Put!"<<endl;
            cout<<"Aktualne punkty: "<<g_Stats.punktyTejRundy<<endl;
        }
        else
        {
            this->velocity = sf::Vector2f(0,0);
            this->setPosition(400,180);
        }

    }

    void animate(const sf::Time &elapsed, std::vector<BilardBall> &Kule, const std::vector<BilardHole> &Dziury, float tarcieScianGlobal, float tarcieStoluGlobal) {

        const float dt = elapsed.asSeconds();
        move(dt*velocity.x, dt*velocity.y);
        rotate(dt*rotation);

        // Spowalnianie przez tarcie
        this->rob_tarcie(tarcieStoluGlobal, 0);

    }

    void kolizjeDziur(const sf::Time &elapsed, std::vector<BilardBall> &Kule, const std::vector<BilardHole> &Dziury, float tarcieScianGlobal, float tarcieStoluGlobal)
    {
        for ( auto &hol : Dziury )
        {
            if ( diff( this->getPosition(), hol.getPosition() ) < this->getRadius() + hol.getRadius() )
            {
                this->ballPut();
            }
        }
    }

    void kolizjeKul(const sf::Time &elapsed, std::vector<BilardBall> &Kule, const std::vector<BilardHole> &Dziury, float tarcieScianGlobal, float tarcieStoluGlobal)
    {

        // Sprawdzanie kolizji kul
        for ( auto &bal : Kule )
        {
            if( bal.identifier != this->identifier && !bal.Put && find( bal.saveBounces.begin(), bal.saveBounces.end(), this->identifier) == bal.saveBounces.end() )
            {
                if ( diff( this->getPosition(), bal.getPosition() ) < this->getRadius() + bal.getRadius() )
                {
                    g_Stats.punktyTejRundy += punktyNaUderzeniuKuli;
                    bal.saveBounces.emplace_back(this->identifier);
                    this->ballhitSFX.play();
                    sf::Vector2f D = this->getPosition() - bal.getPosition();
                    float d = diff(D);
                    float delt = this->getRadius() + bal.getRadius() - d;
                    sf::Vector2f N = normal(D);
                    this->move( 0.5f * delt * N * 1.05f );
                    bal.move( -0.5f * delt * N * 1.05f );
                    float vi = ( bal.velocity.x * N.x ) + ( bal.velocity.y * N.y ) - ( this->velocity.x * N.x ) - ( this->velocity.y * N.y );
                    if(vi >= 0)
                    {
                        this->setVelocity( this->velocity + vi*N );
                        bal.setVelocity( bal.velocity - vi*N );
                    }
                }
            }
        }


    }

    void kolizjeScian(const sf::Time &elapsed, std::vector<BilardBall> &Kule, const std::vector<BilardHole> &Dziury, float tarcieScianGlobal, float tarcieStoluGlobal)
    {
        // Sprawdzenie kolizji ścian
        if(this->getPosition().x + this->getRadius() > 620)
        {
            this->wallhitSFX.play();
            g_Stats.punktyTejRundy += punktyNaUderzeniuSciany;
            this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
            this->move(sf::Vector2f(620.f - this->getPosition().x - this->getRadius() - 2, 0));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().x - this->getRadius() < 20)
        {
            this->wallhitSFX.play();
            g_Stats.punktyTejRundy += punktyNaUderzeniuSciany;
            this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
            this->move(sf::Vector2f(20.f - this->getPosition().x + this->getRadius() + 2, 0));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().y + this->getRadius() > 334)
        {
            this->wallhitSFX.play();
            g_Stats.punktyTejRundy += punktyNaUderzeniuSciany;
            this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
            this->move(sf::Vector2f(0, 334.f - this->getPosition().y - this->getRadius() - 2));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
        if(this->getPosition().y - this->getRadius() < 24)
        {
            this->wallhitSFX.play();
            g_Stats.punktyTejRundy += punktyNaUderzeniuSciany;
            this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
            this->move(sf::Vector2f(0, 24.f - this->getPosition().y + this->getRadius() + 2));
            this->rob_tarcie(tarcieScianGlobal, 1);
        }
    }

    void cleanBounces()
    {
        saveBounces = {};
    }

    void setVelocity(sf::Vector2f vel)
    {
        velocity = vel;
    }

    bool stationary()
    {
        if( abs(this->velocity.x) < 0.1f && abs(this->velocity.y) < 0.1f )
            return true;
        if( this->Put )
            return true;
        return false;
    }
};

void resetKuleForNextRound( std::vector<BilardBall> &Kule, std::vector<float> MiejscaX, std::vector<float> MiejscaY, int &shots  )
{
    for( auto &bal : Kule )
    {
        bal.Put = false;
        bal.setPosition( MiejscaX[ &bal - &Kule[0] ], MiejscaY[ &bal - &Kule[0] ] );
        bal.setVelocity( sf::Vector2f(0,0) );
    }
    shots = 0;
    g_Stats.punktyTejRundy = 0;
    cout<<"* reset board *"<<endl;
}

void mult( std::vector<float> &vec, float num )
{
    for( auto &v : vec )
    {
        v *= num;
    }
}

void resetCalejRozgrywki(std::vector<BilardBall> &Kule,std::vector<float> MiejscaX, std::vector<float> MiejscaY, int &shots)
{
    resetKuleForNextRound(Kule,MiejscaX,MiejscaY,shots);
    g_Stats.punktyGlobalnie=0;
    g_Stats.monety=0;
    g_Stats.monetyGlobalnie=0;
    g_Stats.rundy=1;
    g_Stats.wbiteBileGlobalnie=0;
    g_Stats.strzalyGlobalnie=0;
    std::cout<<"resetujemy runa"<<std::endl;
}

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

    // Stworzenie dziur
    vector <float> pozycjedziurX = {320, 320, 24 ,26 , 614 ,614 };
    vector <float> pozycjedziurY = {23,  330, 27 ,328, 26, 330 };
    mult(pozycjedziurX, (float(virtualScreen.getSize().x)/window.getSize().x));
    mult(pozycjedziurY, (float(virtualScreen.getSize().y)/window.getSize().y));
    vector <BilardHole> Dziury;
    for (int i=0; i<6; i++)
    {
        float radi = 20*(float(virtualScreen.getSize().x)/window.getSize().x);
        sf::Vector2f position(pozycjedziurX[i],pozycjedziurY[i]);
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
        if(i==15)
        {
            Kule[i].setFillColor(sf::Color(255,255,255));
        }
        else
        {
            Kule[i].setFillColor(sf::Color(180,180,180));
        }
    }


    // Zmienne do ulepszeń
    float silaStrzalu = 1;
    float tarcieStoluGlobal = 1;
    float tarcieScianGlobal = 1;
    int maxStrzaly = 2;
    bool widocznoscCelu = true; // do testów, w grze zmienić na false żeby można było kupić

    // Zmienne do działania
    int celPunktow = 10;
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

            // Myszka pozycja
            sf::Vector2i mouse_pos=sf::Mouse::getPosition(window);
            sf::Vector2f mouse_virtual_pos = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());
            p = mouse_virtual_pos;


            if(currentState==MENU)
            {
                if (event.type == sf::Event::MouseMoved)
                {
                    glowneMenu.updateHover(mouse_virtual_pos);
                }
                if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    MenuAction akcja = glowneMenu.handleClick(mouse_virtual_pos);
                    if(akcja==ActionPlay)
                    {
                        resetKuleForNextRound(Kule,pozycjebazoweX,pozycjebazoweY,strzaly);
                        roundIsActive=true;
                        currentState=PLAYING;
                    }
                    else if(akcja==ActionSettings)
                    {
                        std::cout<<"kiedys beda ustawienia"<<std::endl;
                    }
                    else if(akcja==ActionQuit)
                    {
                        window.close();
                    }
                }
            }
            else if(currentState==GAME_OVER)
            {
                if(event.type==sf::Event::MouseMoved)
                {
                    ekranPrzegranej.updateHover(mouse_virtual_pos);
                }
                if(event.type==sf::Event::MouseButtonPressed&&event.mouseButton.button==sf::Mouse::Left)
                {
                    GameOverAction goAkcja = ekranPrzegranej.handleClick(mouse_virtual_pos);

                    if(goAkcja==GO_ActionRetry)
                    {
                        resetCalejRozgrywki(Kule,pozycjebazoweX,pozycjebazoweY,strzaly);
                        roundIsActive=true;
                        currentState=PLAYING;
                    }
                    else if(goAkcja==GO_ActionMenu)
                    {
                        currentState=MENU;
                    }
                }
            }
            else if(currentState==PLAYING){
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {

                        // sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                        // p = window.mapPixelToCoords(mouse_pos, virtualScreen.getView());
                        // sf::Vector2f pp = window.mapPixelToCoords(mouse_pos);
                        //std::cout << "Mouse clicked: " << pp.x << ", " << pp.y << std::endl;
                        //std::cout << "Mouse clicked retro: " << p.x << ", " << p.y << std::endl;

                        // isLeftPressed = true;
                        // // Tymczasowe włączanie gry
                        // if(!roundIsActive)
                        // {
                        //     resetKuleForNextRound(Kule,pozycjebazoweX,pozycjebazoweY, strzaly);
                        //     roundIsActive = true;
                        // }

                        for (auto &bal : Kule)
                        {
                            if( square(bal.getPosition().x - p.x) + square(bal.getPosition().y - p.y) < square(bal.getRadius()) )
                            {
                                bal.Held = true;
                                if(bal.identifier == 15 && areBallsStationary && strzaly < maxStrzaly)
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
                                if(bal.identifier == 15 && areBallsStationary && strzaly < maxStrzaly)
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

        }
            if(currentState==PLAYING)
            {
                // Zakończenie rundy
                if( areBallsStationary && strzaly >= maxStrzaly )
                {
                    roundIsActive = false;
                    g_Stats.punktyGlobalnie += g_Stats.punktyTejRundy;
                    if( g_Stats.punktyTejRundy >= celPunktow )
                    {
                        // Win
                        // std::cout<<"sklep"<<std::endl;
                    }
                    else
                    {
                        // Lose
                        ekranPrzegranej.updateStats(g_Stats);
                        currentState=GAME_OVER;
                        std::cout<<"Przegrana"<<std::endl;
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
                    //cout<<"velc = "<<velc<<endl;
                    addVelocity = sf::Vector2f(0,0);
                    addVelocity.x = (10*velc/dist_cent)*(Kule[15].getPosition().x-p.x) ;
                    addVelocity.y = (10*velc/dist_cent)*(Kule[15].getPosition().y-p.y) ;
                    Kule[15].setVelocity(addVelocity * silaStrzalu);
                    velc = 0;
                    strzaly ++;
                    g_Stats.strzalyGlobalnie++;
                    cout<<"strzal #"<<strzaly<<endl;
                }

                // Sprawdzenie czy można strzelać
                areBallsStationary = true;
                for (auto &bal : Kule)
                {
                    if( !bal.stationary() )
                    {
                        areBallsStationary = false;
                    }
                }

                // Reset kolizji kul
                for (auto &bal : Kule)
                {
                    bal.cleanBounces();
                }
                // Sprawdzenie kolizji kul
                for (auto &bal : Kule)
                {
                    if(!bal.Put)
                        bal.kolizjeKul(elapsed, Kule, Dziury, tarcieScianGlobal, tarcieStoluGlobal);
                }
                for (auto &bal : Kule)
                {
                    if(!bal.Put)
                        bal.kolizjeScian(elapsed, Kule, Dziury, tarcieScianGlobal, tarcieStoluGlobal);
                }
                for (auto &bal : Kule)
                {
                    if(!bal.Put)
                    {
                        bal.kolizjeDziur(elapsed, Kule, Dziury, tarcieScianGlobal, tarcieStoluGlobal);
                    }
                }
                // Przemieszczenie kul
                for (auto &bal : Kule)
                {
                    if(!bal.Put)
                        bal.animate(elapsed, Kule, Dziury, tarcieScianGlobal, tarcieStoluGlobal);
                }
            }

            // Przygotowanie ekranu do renderowania:
            virtualScreen.clear(currentState==MENU ? sf::Color::White : sf::Color::Black);

            if(currentState==MENU)
            {
                virtualScreen.draw(tlo);
                glowneMenu.draw(virtualScreen);
            }
            else if(currentState==GAME_OVER)
            {
                virtualScreen.draw(tlo);
                for (auto &hol : Dziury)
                    virtualScreen.draw(hol);
                virtualScreen.draw(sciany);

                for (auto &bal : Kule)
                {
                    if(!bal.Put)
                    {
                        virtualScreen.draw(bal);
                    }
                }

                ekranPrzegranej.draw(virtualScreen);
            }
            else if(currentState==PLAYING)
            {
                virtualScreen.draw(tlo);
                for (auto &hol : Dziury)
                    virtualScreen.draw(hol);
                virtualScreen.draw(sciany);

                // Render
                if(roundIsActive)
                {
                    for (auto &bal : Kule)
                    {
                        if(!bal.Put)
                        {
                            virtualScreen.draw(bal);
                        }
                    }


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
