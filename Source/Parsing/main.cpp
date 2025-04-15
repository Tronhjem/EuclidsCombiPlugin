#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Scanner.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
    std::cout << "" << std::endl;
    std::cout << "Running interpreter..." << std::endl;
    std::cout << "" << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    std::vector<char> data;
    data.resize(fileSize);

    file.seekg(0, std::ios::beg);
    file.read(&data[0], fileSize);

    Logger* logger = new Logger();
    Scanner* scanner = new Scanner(&data[0], *logger);
    scanner->ScanTokens();
    
    file.close();

    delete logger;
    delete scanner;

    return 0;
}