#include <iostream>

#include "VM.h"
#include "ScopedTimer.h"

int main(int argc, char* argv[])
{
    ScopedTimer allTimer ("Full Program");

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::unique_ptr<VM> vm;
    {
        ScopedTimer timer("Construct classes");
        vm = std::make_unique<VM>();
    }

    const char* filePath = argv[1];
    vm->Prepare(filePath);
    vm->ProcessOpCodes();

    std::cout << "Program end" << std::endl;
    return 0;
}