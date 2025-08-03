#pragma once

template <typename T>
class Stack
{
public:
    Stack() {};
    T Pop()
    {
#if _DEBUG
        assert(stackPointer > 0);
#endif
        return mStack[--stackPointer];
    }
    
    void Push(T i)
    {
#if _DEBUG
        assert(stackPointer + 1 <= 64);
#endif
        mStack[stackPointer++] = i;
    }
    
    void Clear()
    {
        stackPointer = 0;
    }
    
    uChar Top()
    {
        return mStack[stackPointer];
    }

private:
    std::array<T, 64> mStack;
    int stackPointer = 0;
};