#pragma once

#include <vector>

class FileLoader
{
public:
    FileLoader();
    bool LoadFile(std::string& filePath);
    bool SaveFile(std::string& data);
    char* GetFileStart() { return &mData[0]; };

private:
    std::string mData;
    std::string mFilePath;
};
