#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <vector>
#include <string>
#include <song.h>
#include <Windows.h>

using std::vector, std::string, std::wstring;

class FileLoader {
private:
    vector<song> songs;

    bool isMusicFile(const wstring &fileName);
    bool isValidFile(const wstring &filePath);
    void findMusicFiles(const wstring &directory, vector<song> &songs);

public:
    FileLoader();
    void loadSongs(wstring folderPath);
    vector<song> getSongList();
    void sortSongs(vector<song> &songs);
};

#endif // FILELOADER_H
