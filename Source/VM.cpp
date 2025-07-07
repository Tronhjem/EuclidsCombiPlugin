#include "VM.h"
#include "ScopedTimer.h"
#include "EuclideanGenerator.h"
#include <algorithm>

constexpr int DEFAULT_NOTE_DURATION = 11050;
constexpr int MAX_UCHAR_VALUE = 127;

VM::VM() 
{
    mErrorReporting = std::make_unique<ErrorReporting>();
}

bool VM::Prepare(char* data)
{
    Scanner scanner {*mErrorReporting};
    Compiler compiler {scanner.GetTokens(), *mErrorReporting};
    
    bool success = true;
    success = scanner.ScanFile(data);
    if (success)
    {
		success = compiler.Compile(mRuntimeInstructions);
		if(success)
		{
			success = ProcessOpCodes(mRuntimeInstructions);
        }
    }

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
                const int arrayLength = std::clamp(static_cast<int>(mStack.Pop()), 0, 20); // Clamp to array bounds
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
                const int length = std::clamp(static_cast<int>(mStack.Pop()), 0, 20); // Clamp to array bounds
                const int hits = std::clamp(static_cast<int>(mStack.Pop()), 0, length); // Hits can't exceed length
                uChar data[20];
                GenerateEuclideanSequence(&data[0], hits, length);
                
                for (int i = 0; i < length; ++i)
                {
                    mStack.Push(data[i]);
                }
                
                int clampedLength = std::clamp(length, 0, MAX_UCHAR_VALUE);
                mStack.Push(static_cast<uChar>(clampedLength));
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
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    const int index = std::clamp(static_cast<int>(mStack.Pop()), 0, std::numeric_limits<int>::max());
                    // index is working with a modulo inside GetValue so no need to worry about out of bounds value
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
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) + static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));

                break;
            }

            case(OpCode::SUBTRACT):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) - static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));

                break;
            }

            case(OpCode::MULTIPLY):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) * static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));

                break;
            }

            case(OpCode::DIVIDE):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                if (b == 0) {
                    mStack.Push(0); // Division by zero returns 0
                } else {
                    int result = static_cast<int>(a) / static_cast<int>(b);
                    mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
                }

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
                const uChar value = mStack.Pop();
                std::cout << "PRINT: " << static_cast<int>(value) << std::endl;

                break;
            }
                
            case (OpCode::AND):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a & b;
                mStack.Push(result);

                break;
            }

            case (OpCode::OR):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a | b;
                mStack.Push(result);

                break;
            }

            case (OpCode::XOR):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a ^ b;
                mStack.Push(result);

                break;
            }
                
            case(OpCode::GET_RANDOM_IN_RANGE):
            {
                uChar high = static_cast<uChar>(mStack.Pop());
                uChar low = static_cast<uChar>(mStack.Pop());
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

bool VM::Tick(std::vector<StepData>& stepQueue, const int globalCount)
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
                const int arrayLength = std::clamp(static_cast<int>(mStack.Pop()), 0, 20); // Clamp to array bounds
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    mVariables[instruction.mNameValue].SetValue(i, mStack.Pop());
                }
                
                break;
            }
                
            case (OpCode::GENERATE_EUCLID_SEQUENCE):
            {
                const int length = std::clamp(static_cast<int>(mStack.Pop()), 0, 20); // Clamp to array bounds
                const int hits = std::clamp(static_cast<int>(mStack.Pop()), 0, length); // Hits can't exceed length
                uChar data[20];
                GenerateEuclideanSequence(&data[0], hits, length);
                
                for (int i = 0; i < length; ++i)
                {
                    mStack.Push(data[i]);
                }
                
                int clampedLength = std::clamp(length, 0, MAX_UCHAR_VALUE);
                mStack.Push(static_cast<uChar>(clampedLength));
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
                    return false;
                }
                
                break;
            }
                
            case (OpCode::GET_IDENTIFIER_WITH_INDEX):
            {
                if (mVariables.find(instruction.mNameValue) != mVariables.end())
                {
                    const int index = std::clamp(static_cast<int>(mStack.Pop()), 0, std::numeric_limits<int>::max());
                    // index is working with a modulo inside GetValue so no need to worry about out of bounds value
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
                
            case (OpCode::AND):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a & b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::OR):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a | b;
                mStack.Push(result);
                
                break;
            }
                
            case (OpCode::XOR):
            {
                uChar a = static_cast<uChar>(mStack.Pop() > 0);
                uChar b = static_cast<uChar>(mStack.Pop() > 0);
                uChar result = a ^ b;
                mStack.Push(result);
                
                break;
            }
                
            case(OpCode::ADD):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) + static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
                break;
            }

            case(OpCode::SUBTRACT):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) - static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
                break;
            }

            case(OpCode::MULTIPLY):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                int result = static_cast<int>(a) * static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
                break;
            }

            case(OpCode::DIVIDE):
            {
                uChar b = mStack.Pop();
                uChar a = mStack.Pop();
                if (b == 0) 
                {
                    mStack.Push(0); // Division by zero returns 0
                }
                else
                {
                    int result = static_cast<int>(a) / static_cast<int>(b);
                    mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
                }
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
                return true;
                
            default:
                return false;
        }
    }
}

uChar VM::RandomValue(uChar low, uChar high)
{
    if (high <= low) {
        return low; // Safety: if high <= low, return low
    }
    
    // Prevent overflow in calculation
    int calculatedRange = static_cast<int>(high) - static_cast<int>(low) + 1;
    calculatedRange = std::clamp(calculatedRange, 1, MAX_UCHAR_VALUE);
    
    const int value = rand() % calculatedRange;
    int result = value + static_cast<int>(low);
    return static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE));
}
