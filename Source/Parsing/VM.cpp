#include "VM.h"
#include "ScopedTimer.h"

VM::VM() 
{
    mLogger = std::make_unique<Logger>();
    mFileLoader = std::make_unique<FileLoader>();
    mScanner = std::make_unique<Scanner>(*mLogger);
    mCompiler = std::make_unique<Compiler>(mScanner->GetTokens(), *mLogger);
}

void VM::PrepareFile(const char* filePath)
{
    mFileLoader->LoadFile(filePath);
    mScanner->ScanTokens(mFileLoader->GetFileStart());
    mCompiler->Compile();
}

void VM::Run()
{
    ScopedTimer timer("VM Run");

    int currentIndex = 0;
    auto consume = [&]() -> Instruction &
    {
        return mCompiler->GetInstructions()[currentIndex++];
    };

    auto peek = [&]() -> const Instruction &
    {
        return mCompiler->GetInstructions()[currentIndex];
    };

    for(;;)
    {
        Instruction& instruction = consume();

        switch(instruction.opCode)
        {
            case (OpCode::CONSTANT):
            {
                mStack.Push(instruction.mDoubleValue); 
                break;
            }
            
            case (OpCode::SET_IDENTIFIER):
            {
                mVariables[instruction.mNameValue] = mStack.Pop();
                break;
            }

            case(OpCode::GET_IDENTIFIER):
            {
                double value = mVariables[instruction.mNameValue];
                mStack.Push(value);
                break;
            }

            case(OpCode::ADD):
                mStack.Push(mStack.Pop() + mStack.Pop());
                break;

            case(OpCode::SUBTRACT):
                mStack.Push(mStack.Pop() - mStack.Pop()); 
                break;

            case(OpCode::MULTIPLY):
                mStack.Push(mStack.Pop() * mStack.Pop()); 
                break;

            case(OpCode::DIVIDE):
                mStack.Push(mStack.Pop() / mStack.Pop()); 
                break;

            case(OpCode::PRINT):
            {
                double value = mStack.Pop();
                std::cout << "PRINT: " << value << std::endl;
                break;
            }

            case(OpCode::END):
                return;

            default:
                break;
        }
    }
}