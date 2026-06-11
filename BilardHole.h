#ifndef BILARDHOLE_H
#define BILARDHOLE_H
#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class BilardHole : public sf::CircleShape, public GameObject {
public:
    int identifier;
    sf::IntRect bounds;

    BilardHole(float radius, const sf::Vector2f& position);

    // Implementacja metod z GameObject
    void update(const sf::Time& elapsed) override;
    void draw(sf::RenderTexture& target) override;
};

#endif // BILARDHOLE_H
