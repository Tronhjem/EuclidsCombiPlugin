#include "VM.h"
#include "ScopedTimer.h"

VM::VM() 
{
    mErrorReporting = std::make_unique<ErrorReporting>();
    mFileLoader = std::make_unique<FileLoader>();
    mScanner = std::make_unique<Scanner>(*mErrorReporting);
    mCompiler = std::make_unique<Compiler>(mScanner->GetTokens(), *mErrorReporting);
}

void VM::Prepare(const char* filePath)
{
    mFileLoader->LoadFile(filePath);
    mScanner->ScanFile(mFileLoader->GetFileStart());
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

    for(;;)
    {
        const Instruction& instruction = consume();

        switch(instruction.opCode)
        {
            case (OpCode::CONSTANT):
            {
                mStack.Push(instruction.mDoubleValue); 
                break;
            }
            
            case (OpCode::SET_IDENTIFIER_VALUE):
            {
                mVariables[instruction.mNameValue] = mStack.Pop();
                break;
            }

            case (OpCode::SET_IDENTIFIER_ARRAY):
            {
                int arrayLength = (int)mStack.Pop();
                double data[arrayLength];
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    data[i] = mStack.Pop();
                }
                
                break;
            }

            case(OpCode::GET_IDENTIFIER):
            {
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    double value = mVariables[instruction.mNameValue];
                    mStack.Push(value);
                }
                else
                {
                    std::string error = std::string("VM: Variable not defined");
                    mErrorReporting->LogError(error);
                    return;
                }
                break;
            }

            case(OpCode::ADD):
            {
                mStack.Push(mStack.Pop() + mStack.Pop());
                break;
            }

            case(OpCode::SUBTRACT):
            {
                double b = mStack.Pop();
                double a = mStack.Pop();
                mStack.Push(a - b); 
                break;
            }

            case(OpCode::MULTIPLY):
            {
                mStack.Push(mStack.Pop() * mStack.Pop()); 
                break;
            }

            case(OpCode::DIVIDE):
            {
                double b = mStack.Pop();
                double a = mStack.Pop();
                mStack.Push(a / b); 
                break;
            }

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
