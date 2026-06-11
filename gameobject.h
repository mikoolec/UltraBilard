#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#pragma once
#include <SFML/Graphics.hpp>

// Abstrakcyjna klasa bazowa dla obiektów na stole (Bile, Dziury)
class GameObject {
public:
    virtual ~GameObject() = default;

    // Każdy obiekt na stole musi umieć się zaktualizować i narysować
    virtual void update(const sf::Time& elapsed) = 0;
    virtual void draw(sf::RenderTexture& target) = 0;
};

#endif // GAMEOBJECT_H
