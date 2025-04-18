#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "FileLoader.h"
#include "ScopedTimer.h"

FileLoader::FileLoader()
{

}

bool FileLoader::LoadFile(const char* filePath)
{
    std::ifstream *file = new std::ifstream(filePath);

    if (!file) {
        std::cerr << "Could not open file " << filePath << std::endl;
        return false;
    }

    ScopedTimer timer ("Read File");

    file->seekg(0, std::ios::end);
    size_t fileSize = file->tellg();
    data.resize(fileSize);

    file->seekg(0, std::ios::beg);
    file->read(&data[0], fileSize);

    file->close();
    return true;
}