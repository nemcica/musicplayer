#include "audioplayer.h"

AudioPlayer::AudioPlayer() {
}

void AudioPlayer::loadFile(wstring file_name) {

    if (!music.openFromFile(file_name)) {
        throw std::runtime_error("Failed to load music"); // Exit if there's an error
    }


}

void AudioPlayer::play() {
    music.play();
}

void AudioPlayer::pause() {
    music.pause();
}

void AudioPlayer::stop() {
    music.stop();
}

void AudioPlayer::changeVolume(int volume) {
    music.setVolume(volume);
}

int AudioPlayer::getVolume() {
    return music.getVolume();
}

int AudioPlayer::getDuration() {
    int seconds = music.getDuration().asSeconds();
    return seconds;
}

void AudioPlayer::setOffset(int in_value) {
    music.setPlayingOffset(sf::seconds(in_value));
}

int AudioPlayer::getOffset() {
    return music.getPlayingOffset().asSeconds();
}

sf::Music::Status AudioPlayer::getStatus() {
    return music.getStatus();
}

