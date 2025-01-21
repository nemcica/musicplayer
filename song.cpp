#include "song.h"

song::song(wstring artist_name, wstring song_name, wstring path_name):
    artist_name(artist_name), song_name(song_name), path_name(path_name) {
}

bool song::operator<(song &other) {
    if(artist_name == other.artist_name) {
        return song_name < other.song_name;
    }
    else return artist_name < other.artist_name;
}
