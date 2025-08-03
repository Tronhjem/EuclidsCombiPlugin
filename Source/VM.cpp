#include "VM.h"
#include "ScopedTimer.h"
#include "EuclideanGenerator.h"
#include <algorithm>

constexpr int DEFAULT_NOTE_DURATION = 11050;
constexpr int MAX_UCHAR_VALUE = 127;

#define DO_BINARY_OPERATION_AND_PUSH(OP) \
    do { \
        const uChar b = static_cast<uChar>(mStack.Pop() > 0); \
        const uChar a = static_cast<uChar>(mStack.Pop() > 0); \
        const uChar result = a OP b; \
        mStack.Push(result); \
    } while(0)

#define DO_OPERATION_AND_PUSH(OP) \
    do { \
        const uChar b = static_cast<uChar>(mStack.Pop()); \
        const uChar a = static_cast<uChar>(mStack.Pop()); \
        const uChar result = a OP b; \
        mStack.Push(result); \
    } while(0)

#define DO_OPERATION_WITH_CLAMP_AND_PUSH(OP) \
    do { \
        const uChar b = mStack.Pop(); \
        const uChar a = mStack.Pop(); \
        const int result = static_cast<int>(a) OP static_cast<int>(b); \
        mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE))); \
    } while(0)


VM::VM() 
{
    mErrorReporting = std::make_unique<ErrorReporting>();
}

bool VM::Prepare(char* data)
{
    Scanner scanner {*mErrorReporting};
    Compiler compiler {scanner.GetTokens(), *mErrorReporting};
    
    bool success = false;
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

bool VM::ProcessOpCodes(std::vector<Instruction>& instructions)
{
    ScopedTimer timer("VM Process OpCodes");

    int currentIndex = 0;
    auto consume = [&]() -> Instruction &
    {
        return instructions[currentIndex++];
    };

    for(;;)
    {
        const Instruction& instruction = consume();

        switch(instruction.opCode)
        {
            case (OpCode::SET_IDENTIFIER_VALUE):
            {
                uChar value = mStack.Pop();
                std::vector<uChar> vectorData {value};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};

                break;
            }

            case (OpCode::SET_IDENTIFIER_ARRAY):
            {
                const int arrayLength = std::clamp(static_cast<int>(mStack.Pop()), 0, 32);
                uChar data[32];
                for (int i = arrayLength - 1; i >=0; --i)
                {
                    data[i] = mStack.Pop();
                }

                std::vector<uChar> vectorData {data, data + arrayLength};
                mVariables[instruction.mNameValue] = DataSequence{vectorData};

                break;
            }
                
            case (OpCode::NOTE):
            case (OpCode::CC):
                break;

            case(OpCode::END):
                return true;
                
            default:
            {
                if(!ProcessInstruction(instruction, 0))
                    return false;
            }
        }
    }
}

bool VM::Tick(std::vector<SequenceStep>& stepQueue, const int globalCount)
{
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
            case (OpCode::NOTE):
            {
                const StepData channel = mStack.Pop();
                const StepData vel = mStack.Pop();
                const StepData note = mStack.Pop();
                const StepData shouldTrigger = mStack.Pop();
                
                stepQueue.emplace_back(SequenceStep{StepType::NOTE, shouldTrigger, note, vel, channel, DEFAULT_NOTE_DURATION});
                
                break;
            }
                
            case (OpCode::CC):
            {
                const StepData channel = mStack.Pop();
                const StepData ccValue = mStack.Pop();
                const StepData ccNumber = mStack.Pop();
                const StepData shouldTrigger = mStack.Pop();
                
                stepQueue.emplace_back(SequenceStep{StepType::CC, shouldTrigger, ccNumber, ccValue, channel, DEFAULT_NOTE_DURATION});
                
                break;
            }
                
            case(OpCode::END):
                return true;
                
            default:
                if(!ProcessInstruction(instruction, globalCount))
                    return false;
        }
    }
}

uChar VM::RandomValue(const uChar low, const uChar high)
{
    if (high <= low) {
        return low;
    }
    
    int calculatedRange = static_cast<int>(high) - static_cast<int>(low) + 1;
    calculatedRange = std::clamp(calculatedRange, 1, MAX_UCHAR_VALUE);
    
    const int value = rand() % calculatedRange;
    const int result = value + static_cast<int>(low);
    return static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE));
}

bool VM::ProcessInstruction(const Instruction& instruction, const int stepCount)
{
    switch(instruction.opCode)
    {
        case (OpCode::CONSTANT):
        {
            mStack.Push(instruction.mDataValue);
            
            break;
        }
        
        case (OpCode::SET_IDENTIFIER_VALUE):
        {
            const uChar value = mStack.Pop();
            mVariables[instruction.mNameValue].SetValue(0, value);
            
            break;
        }

        case (OpCode::SET_IDENTIFIER_ARRAY):
        {
            const int arrayLength = std::clamp(static_cast<int>(mStack.Pop()), 0, 32);
            for (int i = arrayLength - 1; i >=0; --i)
            {
                mVariables[instruction.mNameValue].SetValue(i, mStack.Pop());
            }
            
            break;
        }
            
        case (OpCode::GENERATE_EUCLID_SEQUENCE):
        {
            const int length = std::clamp(static_cast<int>(mStack.Pop()), 0, 32);
            const int hits = std::clamp(static_cast<int>(mStack.Pop()), 0, length);
            uChar data[32];
            GenerateEuclideanSequence(data, hits, length);
            
            for (int i = 0; i < length; ++i)
            {
                mStack.Push(data[i]);
            }
            
            const int clampedLength = std::clamp(length, 0, MAX_UCHAR_VALUE);
            mStack.Push(static_cast<uChar>(clampedLength));
            break;
        }
            
        case (OpCode::GET_IDENTIFIER_VALUE):
        {
            if (mVariables.find(instruction.mNameValue) != mVariables.end())
            {
                const uChar value = mVariables[instruction.mNameValue].GetValue(stepCount);
                mStack.Push(value);
            }
            else
            {
                const std::string error = std::string("VM: Variable not defined");
                mErrorReporting->LogError(error);
                return false;
            }
            
            break;
        }
            
        case (OpCode::GET_IDENTIFIER_WITH_INDEX):
        {
            if (mVariables.find(instruction.mNameValue) != mVariables.end())
            {
                const int index = mStack.Pop();
                // GetGalue is done with modulo inside, so no need to worry about out of bounds value
                const uChar value = mVariables[instruction.mNameValue].GetValue(index);
                mStack.Push(value);
            }
            else
            {
                const std::string error = std::string("VM: Variable not defined");
                mErrorReporting->LogError(error);
                return false;
            }

            break;
        }
            
        case (OpCode::AND):
        {
            DO_BINARY_OPERATION_AND_PUSH(&);
            
            break;
        }
            
        case (OpCode::OR):
        {
            DO_BINARY_OPERATION_AND_PUSH(|);
            
            break;
        }
            
        case (OpCode::XOR):
        {
            DO_BINARY_OPERATION_AND_PUSH(^);
            
            break;
        }
            
        case(OpCode::ADD):
        {
            DO_OPERATION_WITH_CLAMP_AND_PUSH(+);
            
            break;
        }

        case(OpCode::SUBTRACT):
        {
            DO_OPERATION_WITH_CLAMP_AND_PUSH(-);
            
            break;
        }

        case(OpCode::MULTIPLY):
        {
            DO_OPERATION_WITH_CLAMP_AND_PUSH(*);
            
            break;
        }

        case(OpCode::DIVIDE):
        {
            const uChar b = mStack.Pop();
            const uChar a = mStack.Pop();
            if (b == 0)
            {
                mStack.Push(0); // Division by zero returns 0
            }
            else
            {
                const int result = static_cast<int>(a) / static_cast<int>(b);
                mStack.Push(static_cast<uChar>(std::clamp(result, 0, MAX_UCHAR_VALUE)));
            }
            
            break;
        }
            
        case(OpCode::LESS):
        {
            DO_OPERATION_AND_PUSH(<);
            
            break;
        }
            
        case(OpCode::LESS_EQUAL):
        {
            DO_OPERATION_AND_PUSH(<=);

            break;
        }
            
        case(OpCode::GREATER):
        {
            DO_OPERATION_AND_PUSH(>);

            break;
        }
            
        case(OpCode::GREATER_EQUAL):
        {
            DO_OPERATION_AND_PUSH(>=);
            
            break;
        }
            
        case(OpCode::EQUAL):
        {
            DO_OPERATION_AND_PUSH(==);

            break;
        }
            
        case(OpCode::GET_RANDOM_IN_RANGE):
        {
            const uChar high = mStack.Pop();
            const uChar low = mStack.Pop();
            const uChar value = RandomValue(low, high);
            mStack.Push(value);
            
            break;
        }
            
        case(OpCode::PRINT):
        {
            const uChar value = mStack.Pop();
            const std::string message = "PRINT: " + std::to_string(static_cast<int>(value));
            std::cout << message << std::endl;
            mErrorReporting->LogMessage(message);

            break;
        }
            
        default:
            const std::string err {"Unexpected Operation code"};
            mErrorReporting->LogError(err);
            
            return false;
    }
    
    return true;
}
