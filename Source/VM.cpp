#include "VM.h"
#include "ScopedTimer.h"
#include "EuclideanGenerator.h"

VM::VM() 
{
    mErrorReporting = std::make_unique<ErrorReporting>();
}

bool VM::Prepare(char* data)
{
    Scanner scanner {*mErrorReporting};
    Compiler compiler {scanner.GetTokens(), *mErrorReporting};
    
    bool success = true;
    if (success &= scanner.ScanFile(data))
        if(success &= compiler.Compile(mRuntimeInstructions))
            success &= ProcessOpCodes(compiler.GetSetupInstructions());
        
    return success;
}

void VM::Reset()
{
    mVariables.clear();
    mRuntimeInstructions.clear();
}

bool VM::ProcessOpCodes(std::vector<Instruction>& setupInstructions)
{
    ScopedTimer timer("VM Process OpCodes");

    int currentIndex = 0;
    auto consume = [&]() -> Instruction &
    {
        return setupInstructions[currentIndex++];
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
                const int arrayLength = (int) mStack.Pop();
                uChar data[20];
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    data[i] = mStack.Pop();
                }

                std::vector<uChar> vectorData {data, data + arrayLength};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};
                
                break;
            }
                
            case (OpCode::GENERATE_EUCLID_SEQUENCE):
            {
                const int length = (int) mStack.Pop();
                const int hits = (int) mStack.Pop();
                uChar data[20];
                GenerateEuclideanSequence(&data[0], hits, length);
                
                std::vector<uChar> vectorData {data, data + length};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};
            }

            case(OpCode::GET_IDENTIFIER_VALUE):
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
                    return false;
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
                    return false;
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
                
                return true;

            default:
                std::string err {"Unexpected Operation code"};
                mErrorReporting->LogError(err);
                
                return false;
        }
    }
}

void VM::Tick(MidiScheduler& midiScheduler, int nextTickTime, int globalCount)
{
//    ScopedTimer timer("VM Runtime");
    
    int currentIndex = 0;
    auto consume = [&]() -> Instruction&
    {
        return mRuntimeInstructions[currentIndex++];
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
                
            case (OpCode::GET_IDENTIFIER_VALUE):
            {
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    uChar value = mVariables[instruction.mNameValue].GetValue(globalCount);
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
                
            case (OpCode::AND):
            {
                uChar a = mStack.Pop();
                uChar b = mStack.Pop();
                uChar result = a & b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::OR):
            {
                uChar a = mStack.Pop();
                uChar b = mStack.Pop();
                uChar result = a | b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::XOR):
            {
                uChar a = mStack.Pop();
                uChar b = mStack.Pop();
                uChar result = a ^ b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::NOTE):
            {
                const int channel = (int) mStack.Pop();
                const int vel = (int) mStack.Pop();
                const int note = (int) mStack.Pop();
                const int shouldTrigger = (int) mStack.Pop();
                
                if (shouldTrigger > 0)
                {
                    midiScheduler.PostMidiNote(channel, note, vel, 11050, nextTickTime);
                }
                
                break;
            }
                
            case (OpCode::CC):
            {
                const int channel = (int) mStack.Pop();
                const int ccValue = (int) mStack.Pop();
                const int ccNumber = (int) mStack.Pop();
                const int shouldTrigger = (int) mStack.Pop();
                
                if (shouldTrigger > 0)
                {
                    midiScheduler.PostMidiCC(channel, ccNumber, ccValue, nextTickTime);
                }
                
                break;
            }
                
            case(OpCode::END):
                return;
                
            default:
                return;
        }
    }
}
