#include "VM.h"
#include "ScopedTimer.h"

VM::VM() 
{
    mErrorReporting = std::make_unique<ErrorReporting>();
    mFileLoader = std::make_unique<FileLoader>();
    mScanner = std::make_unique<Scanner>(*mErrorReporting);
    mCompiler = std::make_unique<Compiler>(mScanner->GetTokens(), *mErrorReporting);
}

bool VM::Prepare(const char* filePath)
{
    bool success = true;
    if(success &= mFileLoader->LoadFile(filePath))
    {
        if (success &= mScanner->ScanFile(mFileLoader->GetFileStart()))
            success &= mCompiler->Compile();
    }

    return success;
}

void VM::ProcessOpCodes()
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
                mStack.Push(instruction.mDataValue);
                break;
            }
            
            case (OpCode::SET_IDENTIFIER_VALUE):
            {
                uChar value = mStack.Pop();
                std::vector<uChar> vectorData {value};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};
                break;
            }

            case (OpCode::SET_IDENTIFIER_ARRAY):
            {
                int arrayLength = (int) mStack.Pop();
                uChar data[arrayLength];
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    data[i] = mStack.Pop();
                }

                std::vector<uChar> vectorData {data, data + arrayLength};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};
                break;
            }

            case(OpCode::GET_IDENTIFIER):
            {
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    uChar value = mVariables[instruction.mNameValue].GetValue(0);
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

            case (OpCode::GET_IDENTIFIER_WITH_INDEX):
            {
                int index = (int) mStack.Pop();
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    uChar value = mVariables[instruction.mNameValue].GetValue(index);
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
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
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
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a / b); 
                break;
            }

            case(OpCode::PRINT):
            {
                uChar value = mStack.Pop();
                std::cout << "PRINT: " << (int)value << std::endl;
                break;
            }

            case(OpCode::END):
                return;

            default:
                std::string err {"Unexpected Operation code"};
                mErrorReporting->LogError(err);
                break;
        }
    }
}

void VM::Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
{

}
