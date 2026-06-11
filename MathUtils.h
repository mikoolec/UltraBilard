#ifndef MATHUTILS_H
#define MATHUTILS_H
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

inline float square(float a) {
    return a * a;
}

inline float diff(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt(square(a.x - b.x) + square(a.y - b.y));
}

inline float diff(sf::Vector2f a) {
    return std::sqrt(square(a.x) + square(a.y));
}

inline sf::Vector2f normal(sf::Vector2f V) {
    float len = diff(V);
    if (len == 0) return sf::Vector2f(0, 0);
    return V / len;
}
#endif // MATHUTILS_H
