#include "BilardBall.h"
#include "MathUtils.h"
#include "AudioManager.h"
#include "Globals.h"
#include <iostream>
#include <cmath>

using namespace std;

BilardBall::BilardBall(float radius, const sf::Vector2f& position, int id)
    : sf::CircleShape(radius), velocity(sf::Vector2f(0,0)), bounds(sf::IntRect(0,0,0,0)), identifier(id)
{
    setPosition(position);
    setOrigin(sf::Vector2f(radius, radius));
    rotation = 0;

    resetBaseStats();

    wallhitSFX.setBuffer(g_Audio.wallhitBuffer);
    wallhitSFX.setVolume(20.f);
    ballhitSFX.setBuffer(g_Audio.ballhitBuffer);
    ballhitSFX.setVolume(15.f);

    // Ustawienie początkowych statystyk
    resetStats();
}

void BilardBall::resetBaseStats() {
    tarcieBaza = 0.125f;
    tarciescianBaza = 5.f;
    punktyBaza = 1;
    mnoznikBaza = 1;
    wielkoscBaza = 1;
    hitpunktyBaza = 0;
    hitscianaBaza = 0;
    monetyBaza = 0;
    hitmonetyBaza = 0;
}

void BilardBall::resetStats() {
    this->tarcie = this->tarcieBaza;
    this->tarciescian = this->tarciescianBaza;
    this->wartoscPunktowa = this->punktyBaza;
    this->mnoznikPunktowy = this->mnoznikBaza;
    this->radiusMultipl = this->wielkoscBaza;
    this->punktyNaUderzeniuKuli = this->hitpunktyBaza;
    this->punktyNaUderzeniuSciany = this->hitscianaBaza;
}

void BilardBall::rob_tarcie(float sila, bool sciana) {
    sf::Vector2f velc_po_tarc = this->velocity - (normal(this->velocity) * (sciana ? this->tarciescian : this->tarcie) * sila);
    if (this->velocity.x * velc_po_tarc.x < 0) velc_po_tarc.x = 0.f;
    if (this->velocity.y * velc_po_tarc.y < 0) velc_po_tarc.y = 0.f;
    this->setVelocity(velc_po_tarc);
}

void BilardBall::ballPut() {
    if (this->identifier != 15) {
        g_Stats.punktyTejRundy += this->wartoscPunktowa;
        g_Stats.punktyTejRundy *= this->mnoznikPunktowy;
        if (this->monetyBaza != 0) {
            g_Stats.monety += this->monetyBaza;
            g_Stats.monetyGlobalnie += this->monetyBaza;
        }
        g_Stats.wbiteBileGlobalnie += 1;
        this->Put = true;
    } else {
        // Biała bila wraca na stół
        this->velocity = sf::Vector2f(0, 0);
        this->setPosition(400, 180);
    }
}

// Zastępuje dawną metodę animate()
void BilardBall::update(const sf::Time& elapsed) {
    if (Put) return;
    const float dt = elapsed.asSeconds();
    move(dt * velocity.x, dt * velocity.y);
    rotate(dt * rotation);

    // Zamiast podawać tarcieStoluGlobal co klatkę, zakładamy, że bazowo to 1.0f
    // (Możesz to w przyszłości przenieść do Globals.h jeśli będzie taka potrzeba)
    this->rob_tarcie(1.0f, false);
}

void BilardBall::draw(sf::RenderTexture& target) {
    if (Put) return;

    target.draw(*this);

    // Pobieramy ulepszenia tej bili z g_Stats
    if (identifier >= 0 && identifier < 15) {
        const auto& ulepszenia = g_Stats.ulepszeniaBil[identifier];
        int ilosc = ulepszenia.size();

        if (ilosc > 0) {
            float promienKropki = 2.0f;

            // POPRAWKA: Używamy getRadius() zamiast zmiennej radius
            float odlegloscOdSrodka = getRadius() - 3.5f;

            for (int i = 0; i < ilosc; i++) {
                int id = ulepszenia[i];
                sf::Color kolor(255, 255, 255);

                switch (id) {
                    case 201: kolor = sf::Color(80, 80, 80); break;   // Kowadlo (Szary)
                    case 202: kolor = sf::Color(135, 206, 235); break; // Balon (Błękit)
                    case 203: kolor = sf::Color(139, 69, 19); break;  // Kamien (Brąz)
                    case 204: kolor = sf::Color(220, 20, 60); break;  // Bomba (Czerwień)
                    case 205: kolor = sf::Color(50, 205, 50); break;  // Sprezyna (Zieleń)
                    case 206: kolor = sf::Color(0, 255, 255); break;  // Lód (Cyjan)
                    case 207: kolor = sf::Color(255, 215, 0); break;  // Klej (Żółty)
                    case 208: kolor = sf::Color(138, 43, 226); break; // Magnes (Fiolet)
                    case 209: kolor = sf::Color(255, 255, 255); break;// Duch (Biały)
                    case 210: kolor = sf::Color(255, 140, 0); break;  // Ogień (Pomarańcz)
                    case 211: kolor = sf::Color(0, 191, 255); break;  // Diament (Niebieski)
                    case 212: kolor = sf::Color(255, 223, 0); break;  // Gwiazda (Złoto)
                    case 213: kolor = sf::Color(178, 34, 34); break;  // Cel (Ciemna czerwień)
                    case 214: kolor = sf::Color(218, 165, 32); break; // Korona (Stare złoto)
                    case 215: kolor = sf::Color(255, 105, 180); break;// Skarbonka (Różowy)
                    case 216: kolor = sf::Color(34, 139, 34); break;  // Inwestycja (Ciemna zieleń)
                    case 217: kolor = sf::Color(192, 192, 192); break;// Szkło (Jasnoszary)
                }

                sf::CircleShape kropka(promienKropki);
                kropka.setFillColor(kolor);
                kropka.setOrigin(promienKropki, promienKropki);
                kropka.setOutlineThickness(0.5f);
                kropka.setOutlineColor(sf::Color(0,0,0, 150));

                float kat = (i * (360.0f / ilosc)) * 3.14159f / 180.0f;

                // POPRAWKA: Używamy getPosition() zamiast shape.getPosition()
                float x = getPosition().x + std::cos(kat) * odlegloscOdSrodka;
                float y = getPosition().y + std::sin(kat) * odlegloscOdSrodka;

                kropka.setPosition(x, y);
                target.draw(kropka);
            }
        }
    }
}
void BilardBall::kolizjeDziur(const sf::Time& elapsed, std::vector<std::unique_ptr<GameObject>>& entities, float tarcieScianGlobal, float tarcieStoluGlobal) {
    for (auto& obj : entities) {
        // MAGIA! Sprawdzamy, czy ten obiekt to Dziura
        BilardHole* hol = dynamic_cast<BilardHole*>(obj.get());
        if (hol) {
            if (diff(this->getPosition(), hol->getPosition()) < this->getRadius() + hol->getRadius()) {
                this->ballPut();
            }
        }
    }
}

void BilardBall::kolizjeKul(const sf::Time& elapsed, std::vector<std::unique_ptr<GameObject>>& entities, float tarcieScianGlobal, float tarcieStoluGlobal) {
    for (auto& obj : entities) {
        // MAGIA! Sprawdzamy, czy ten obiekt to inna Bila
        BilardBall* bal = dynamic_cast<BilardBall*>(obj.get());

        if (bal && bal->identifier != this->identifier && !bal->Put &&
            find(bal->saveBounces.begin(), bal->saveBounces.end(), this->identifier) == bal->saveBounces.end())
        {
            if (diff(this->getPosition(), bal->getPosition()) < this->getRadius() + bal->getRadius()) {
                g_Stats.punktyTejRundy += this->punktyNaUderzeniuKuli;
                if (this->hitmonetyBaza != 0) {
                    g_Stats.monety += this->hitmonetyBaza;
                    g_Stats.monetyGlobalnie += this->hitmonetyBaza;
                }
                bal->saveBounces.emplace_back(this->identifier);
                this->ballhitSFX.play();

                sf::Vector2f D = this->getPosition() - bal->getPosition();
                float d = diff(D);
                float delt = this->getRadius() + bal->getRadius() - d;
                sf::Vector2f N = normal(D);

                this->move(0.5f * delt * N * 1.05f);
                bal->move(-0.5f * delt * N * 1.05f);

                float vi = (bal->velocity.x * N.x) + (bal->velocity.y * N.y) - (this->velocity.x * N.x) - (this->velocity.y * N.y);
                if (vi >= 0) {
                    this->setVelocity(this->velocity + vi * N);
                    bal->setVelocity(bal->velocity - vi * N);
                }
            }
        }
    }
}

void BilardBall::kolizjeScian(const sf::Time& elapsed, float tarcieScianGlobal, float tarcieStoluGlobal) {
    // Sprawdzenie kolizji ścian (logika pozostaje bez zmian)
    if(this->getPosition().x + this->getRadius() > 620) {
        this->wallhitSFX.play();
        this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
        this->move(sf::Vector2f(620.f - this->getPosition().x - this->getRadius() - 2, 0));
        this->rob_tarcie(tarcieScianGlobal, true);
    }
    if(this->getPosition().x - this->getRadius() < 20) {
        this->wallhitSFX.play();
        this->setVelocity(sf::Vector2f(-1*this->velocity.x, this->velocity.y));
        this->move(sf::Vector2f(20.f - this->getPosition().x + this->getRadius() + 2, 0));
        this->rob_tarcie(tarcieScianGlobal, true);
    }
    if(this->getPosition().y + this->getRadius() > 334) {
        this->wallhitSFX.play();
        this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
        this->move(sf::Vector2f(0, 334.f - this->getPosition().y - this->getRadius() - 2));
        this->rob_tarcie(tarcieScianGlobal, true);
    }
    if(this->getPosition().y - this->getRadius() < 24) {
        this->wallhitSFX.play();
        this->setVelocity(sf::Vector2f(this->velocity.x, -1*this->velocity.y));
        this->move(sf::Vector2f(0, 24.f - this->getPosition().y + this->getRadius() + 2));
        this->rob_tarcie(tarcieScianGlobal, true);
    }
}

void BilardBall::cleanBounces() {
    saveBounces.clear();
}

void BilardBall::setVelocity(sf::Vector2f vel) {
    velocity = vel;
}

bool BilardBall::stationary() {
    if (abs(this->velocity.x) < 0.1f && abs(this->velocity.y) < 0.1f) return true;
    if (this->Put) return true;
    return false;
}

sf::Vector2f BilardBall::getVelocity() const {
    return velocity;
}
