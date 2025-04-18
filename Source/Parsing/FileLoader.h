#pragma once
#include <vector>

class FileLoader
{
public:
    FileLoader();
    bool LoadFile(const char *filePath);
    char* GetFileStart() { return &data[0]; };

private:
    std::vector<char> data;
};
