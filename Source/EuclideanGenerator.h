#pragma once
#include "DataSequence.h"

void GenerateEuclideanSequence(uChar* sequence, const int hits, const int length)
{
// Python reference
//    def generate_sequence(self):
//        count = self._length
//
//        for x in range(self._length):
//            if count >= self._length:
//                self._sequence[x] = 1
//                count = count - self._length
//
//            else:
//                self._sequence[x] = 0
//
//            count = count + self.hits
    
    
    int count = length;
    for(int i = 0; i < length; ++i)
    {
        if (count >= length)
        {
            sequence[i] = static_cast<uChar>(1);
            count = count - length;
        }
        else
        {
            sequence[i] = static_cast<uChar>(0);
        }
        count += hits;
    }
}
