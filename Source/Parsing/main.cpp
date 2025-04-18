#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "Scanner.h"
#include "Logger.h"
#include "Compiler.h"
#include "VM.h"
#include "ScopedTimer.h"
#include "CustomMemory.h"

int main(int argc, char* argv[])
{
    ScopedTimer allTimer ("Full Program");

    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    //     return 1;
    // }

    // std::ifstream file(argv[1]);
    std::ifstream *file = new std::ifstream("./myFile.lox");

    std::unique_ptr<Logger> logger;
    std::unique_ptr<Scanner> scanner;
    std::unique_ptr<Compiler> compiler;
    std::unique_ptr<VM> vm;

    if (!file) {
        std::cerr << "Could not open file " << argv[1] << std::endl;
        return 1;
    }
    // Logger* another = new Logger();

    std::vector<char> data;
    {
        ScopedTimer timer ("Read File");
        file->seekg(0, std::ios::end);
        size_t fileSize = file->tellg();
        data.resize(fileSize);

        file->seekg(0, std::ios::beg);
        file->read(&data[0], fileSize);
    }
    

    {
        ScopedTimer timer ("Construct classes");
        logger = std::make_unique<Logger>();
        scanner = std::make_unique<Scanner>(&data[0], *logger);
        compiler = std::make_unique<Compiler>(scanner->GetTokens(), *logger);
        vm = std::make_unique<VM>(compiler->GetInstructions(), *logger);
    }

    if(scanner->ScanTokens())
    {
        if(compiler->Compile())
            vm->Run();
    }

    
    file->close();
    delete file;

    std::cout << "Program end" << std::endl;
    return 0;
}