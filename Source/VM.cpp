#include "VM.h"
#include "ScopedTimer.h"
#include "EuclideanGenerator.h"

constexpr int DEFAULT_NOTE_DURATION = 11050;

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
            success &= ProcessOpCodes(mRuntimeInstructions);
        
    mStack.Clear();
    return success;
}

void VM::Reset()
{
    mVariables.clear();
    mRuntimeInstructions.clear();
    mErrorReporting->Clear();
}

std::vector<LogEntry>& VM::GetErrors()
{
    return mErrorReporting->GetErrors();
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
                
                break;
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
                
            case(OpCode::LESS):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a < b);

                break;
            }
                
            case(OpCode::LESS_EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a <= b);

                break;
            }
                
            case(OpCode::GREATER):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a > b);

                break;
            }
                
            case(OpCode::GREATER_EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a >= b);

                break;
            }
                
            case(OpCode::EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a == b);

                break;
            }

            case(OpCode::PRINT):
            {
                uChar value = mStack.Pop();
                std::cout << "PRINT: " << (int)value << std::endl;

                break;
            }

            case (OpCode::AND):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a & b;
                mStack.Push(result);

                break;
            }

            case (OpCode::OR):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a | b;
                mStack.Push(result);

                break;
            }

            case (OpCode::XOR):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a ^ b;
                mStack.Push(result);

                break;
            }
                
            case(OpCode::GET_RANDOM_IN_RANGE):
            {
                uChar high = mStack.Pop();
                uChar low = mStack.Pop();
                mStack.Push(RandomValue(low, high));
                break;
            }
                
            case (OpCode::NOTE):
            case (OpCode::CC):
                break;

            case(OpCode::END):
                return true;

            default:
                std::string err {"Unexpected Operation code"};
                mErrorReporting->LogError(err);

                return false;
        }
    }
}

void VM::Tick(std::vector<StepData>& stepQueue, const int globalCount)
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
            
            case (OpCode::SET_IDENTIFIER_VALUE):
            {
                uChar value = mStack.Pop();
                mVariables[instruction.mNameValue].SetValue(0, value);
                
                break;
            }

            case (OpCode::SET_IDENTIFIER_ARRAY):
            {
                const int arrayLength = (int) mStack.Pop();
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    mVariables[instruction.mNameValue].SetValue(i, mStack.Pop());
                }
                
                break;
            }
                
            case (OpCode::GENERATE_EUCLID_SEQUENCE):
            {
                const int length = (int) mStack.Pop();
                const int hits = (int) mStack.Pop();
                uChar data[length];
                GenerateEuclideanSequence(&data[0], hits, length);
                
                std::vector<uChar> vectorData {data, data + length};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};
                
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
                
            case (OpCode::AND):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a & b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::OR):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a | b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::XOR):
            {
                uChar a = (uChar) (mStack.Pop() > 0);
                uChar b = (uChar) (mStack.Pop() > 0);
                uChar result = a ^ b;
                mStack.Push(result);
                
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
                
            case(OpCode::LESS):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a < b);

                break;
            }
                
            case(OpCode::LESS_EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a <= b);

                break;
            }
                
            case(OpCode::GREATER):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a > b);

                break;
            }
                
            case(OpCode::GREATER_EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a >= b);

                break;
            }
                
            case(OpCode::EQUAL):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                mStack.Push(a == b);

                break;
            }
                
            case(OpCode::GET_RANDOM_IN_RANGE):
            {
                uChar high = mStack.Pop();
                uChar low = mStack.Pop();
                mStack.Push(RandomValue(low, high));
                break;
            }
                
            case (OpCode::NOTE):
            {
                const uChar channel = mStack.Pop();
                const uChar vel = mStack.Pop();
                const uChar note = mStack.Pop();
                const uChar shouldTrigger = mStack.Pop();
                
                stepQueue.emplace_back(StepData{StepType::NOTE, shouldTrigger, note, vel, channel, DEFAULT_NOTE_DURATION});
            
                break;
            }
                
            case (OpCode::CC):
            {
                const uChar channel = mStack.Pop();
                const uChar ccValue = mStack.Pop();
                const uChar ccNumber = mStack.Pop();
                const uChar shouldTrigger = mStack.Pop();
                
                stepQueue.emplace_back(StepData{StepType::CC, shouldTrigger, ccNumber, ccValue, channel, DEFAULT_NOTE_DURATION});
                
                break;
            }
                
            case(OpCode::END):
                return;
                
            default:
                return;
        }
    }
}

uChar VM::RandomValue(uChar low, uChar high)
{
    assert(high > low);
    uChar calculatedHighValue = high + 1 - low;
    int value = rand() % calculatedHighValue;
    return (uChar) value + low;
}
