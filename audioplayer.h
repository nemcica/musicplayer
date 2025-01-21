#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "SFML/Audio.hpp"
#include <iostream>

using std::wstring;

class AudioPlayer {
private:
   sf::Music music;

public:
    AudioPlayer();
    void loadFile(wstring file_name);
    void play();
    void pause();
    void stop();
    void changeVolume(int volume);
    int getVolume();
    int getDuration();
    void setOffset(int in_value);
    int getOffset();
    sf::Music::Status getStatus();
};

#endif // AUDIOPLAYER_H
