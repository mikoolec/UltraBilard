#include "AudioManager.h"

// Definicja zmiennej globalnej
AudioManager g_Audio;

void AudioManager::init() {
    if (!wallhitBuffer.loadFromFile("assets//sound_wall.wav")) {
        std::cout << "Blad: Nie znaleziono assets/sound_wall.wav!" << std::endl;
    }
    if (!ballhitBuffer.loadFromFile("assets//sound_ball.wav")) {
        std::cout << "Blad: Nie znaleziono assets/sound_ball.wav!" << std::endl;
    }
}