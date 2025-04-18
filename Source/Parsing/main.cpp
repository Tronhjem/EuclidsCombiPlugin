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

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    std::unique_ptr<VM> vm;

    {
        ScopedTimer timer ("Construct classes");
        vm = std::make_unique<VM>();
    }

    vm->PrepareFile(argv[1]);
    vm->Run();
    
    std::cout << "Program end" << std::endl;
    return 0;
}