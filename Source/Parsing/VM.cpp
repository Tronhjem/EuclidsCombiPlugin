#include "VM.h"
#include "Compiler.h"
#include "Logger.h"
#include "ScopedTimer.h"

#include <iostream>

void VM::Run()
{
    ScopedTimer timer("VM Run");

    auto consume = [&]() -> Instruction&
    {
        return mInstructions[mCurrentIndex++];
    };

    auto peek = [&]() -> const Instruction&
    {
        return mInstructions[mCurrentIndex];
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