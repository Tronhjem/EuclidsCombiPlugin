#pragma once

#include <cassert>
#include <cmath>

typedef unsigned char uChar;

constexpr int MAX_SUB_DIVISION_LENGTH = 4;

class DataSequenceStep
{
public:
    DataSequenceStep() : mLength(0) {};
    DataSequenceStep(const uChar* data, const int length);
    DataSequenceStep(const int i);
    uChar GetValue(const int index) const;
    uChar GetEquivalentValueAtIndex(const int index, const int otherLength) const;
    void SetData(const uChar* data, const int length);
    uChar GetLength() const { return mLength; };
    
    DataSequenceStep operator+ (const DataSequenceStep& other);
    DataSequenceStep operator- (const DataSequenceStep& other);
    DataSequenceStep operator* (const DataSequenceStep& other);
    DataSequenceStep operator/ (const DataSequenceStep& other);
    
    DataSequenceStep operator& (const DataSequenceStep& other);
    DataSequenceStep operator^ (const DataSequenceStep& other);
    DataSequenceStep operator| (const DataSequenceStep& other);
    
private:
    uChar mLength;
    uChar mData[MAX_SUB_DIVISION_LENGTH];
    
    template<typename Operation>
    DataSequenceStep ApplyOperation(const DataSequenceStep& otherSequence,
                                    Operation OperationLambda)
    {
        static_assert(std::is_invocable_v<Operation, int, int>,
                             "Operation must be callable with two int parameters");
        
        const DataSequenceStep& longest = this->GetLength() > otherSequence.GetLength() ? *this : otherSequence;
        const DataSequenceStep& shortest = this->GetLength() <= otherSequence.GetLength() ? *this : otherSequence;
        
        const int newLength = longest.GetLength();
        DataSequenceStep newStep {};
        newStep.mLength = newLength;
        
        for(int i = 0; i < newLength; ++i)
        {
            const int newValue = OperationLambda(shortest.GetEquivalentValueAtIndex(i, newLength), longest.GetValue(i));
            newStep.mData[i] = static_cast<uChar>(std::clamp(newValue, 0, 127));
        }
        
        return newStep;
    }
    
};



