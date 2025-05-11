#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "FileLoader.h"
#include "ScopedTimer.h"

FileLoader::FileLoader()
{
}

bool FileLoader::LoadFile(std::string& filePath)
{
    mFilePath = filePath;
    std::ifstream file (mFilePath);

    if (!file) {
        std::cerr << "Could not open file " << filePath << std::endl;
        return false;
    }

    ScopedTimer timer("Read File");

    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    mData.resize(fileSize);
    file.read(&mData[0], fileSize);

    file.close();
    return true;
}

bool FileLoader::SaveFile(std::string& data)
{
    mData = data;
    
    std::ofstream file {mFilePath, std::ios::binary};
    
    if (!file.is_open()) {
        std::cerr << "Could not open file for writing: " << mFilePath << std::endl;
        return false;
    }

    ScopedTimer timer("Write File");

    file.write(mData.c_str(), static_cast<std::streamsize>(data.size()));
               
    file.close();
    return true;
}
