#ifndef SONG_H
#define SONG_H

#include <iostream>
#include <string>

using std::wstring;

class song {
public:
    wstring artist_name;
    wstring song_name;
    wstring path_name;

public:
    song(wstring artist_name, wstring song_name, wstring path_name);

    bool operator<(song &other);
};

#endif // SONG_H
