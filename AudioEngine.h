//
// Created by sageblatt on 13.04.2022.
//
#ifndef ALIEN_AUDIOENGINE_H
#define ALIEN_AUDIOENGINE_H

#include <SFML/Audio.hpp>
#include <array>

using std::unique_ptr;

enum Songs {
    STARTINGMENU,
    MAINMENU,
    LOR,
    FIGHT,
    TRACKS_AMOUNT
};

class AudioEngine {
private:
    std::array<std::unique_ptr<sf::Music>, TRACKS_AMOUNT> tracks;

    Songs current_track;

    std::array<int, 2> fade_flag;

    const float FADE_LEN = 0.8;
    unique_ptr<sf::Clock> timer;

    void fade(int next_song);

    float volume;

public:
    AudioEngine();

    void run();

    void setFadeFlag(Songs number_of_track);
};


#endif //ALIEN_AUDIOENGINE_H
