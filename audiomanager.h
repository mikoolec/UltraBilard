#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

struct AudioManager {
    sf::SoundBuffer wallhitBuffer;
    sf::SoundBuffer ballhitBuffer;

    void init();
};

// Zmienna globalna dla dźwięków
extern AudioManager g_Audio;

#endif // AUDIOMANAGER_H
