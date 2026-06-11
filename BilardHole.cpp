#include "BilardHole.h"

BilardHole::BilardHole(float radius, const sf::Vector2f& position)
    : sf::CircleShape(radius), bounds(sf::IntRect(0,0,0,0))
{
    setPosition(position);
    setOrigin(sf::Vector2f(radius, radius));
    setFillColor(sf::Color(0, 0, 0));
}

void BilardHole::update(const sf::Time& elapsed) {
    // Dziura się nie porusza, więc update jest puste,
}

void BilardHole::draw(sf::RenderTexture& target) {
    // Rysujemy samych siebie bo dziedziczymy po sf::CircleShape
    target.draw(*this);
}