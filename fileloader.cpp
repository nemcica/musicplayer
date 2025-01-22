#include "fileloader.h"
#include "SFML/Audio.hpp"
#include <QDir>

FileLoader::FileLoader() {

}

bool FileLoader::isMusicFile(const wstring &fileName) {
    if(fileName.find(L".mp3") != wstring::npos) {
        return true;
    }
    return false;
}

void FileLoader::findMusicFiles(const wstring &directory, vector<song> &songs) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    std::wstring searchPath = directory + L"\\*";

    hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        const std::wstring fileName = findFileData.cFileName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (fileName != L"." && fileName != L"..") {
                findMusicFiles(directory + L"\\" + fileName, songs);
            }
        } else {
            if (isMusicFile(fileName)) {
                wstring filePath = directory + L"\\" + fileName;

                wstring delimiter = L" - ";
                size_t pos = fileName.find(delimiter);
                wstring artist;
                wstring songname;
                if (pos != std::wstring::npos) {
                    artist = fileName.substr(0, pos);
                    songname = fileName.substr(pos + 3);
                } else {
                    artist = L"";
                    songname = fileName;
                }
                songs.push_back(song(artist, songname, filePath));
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void FileLoader::loadSongs(wstring folderPath) {
    findMusicFiles(folderPath, songs);
    sortSongs(songs);
}

vector<song> FileLoader::getSongList() {
    return songs;
}

void FileLoader::sortSongs(vector<song> &songs) {
    std::sort(songs.begin(), songs.end());
}
