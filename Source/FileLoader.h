#pragma once

#include <vector>

class FileLoader
{
public:
    FileLoader();
    bool LoadFile(const char *filePath);
    char* GetFileStart() { return &data[0]; };

private:
    //TODO: is it really the best to have this in a vector??
    std::vector<char> data;
};
