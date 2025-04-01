#pragma once

//unsigned int WriteArrayToBytes(unsigned int* start, unsigned int length)
//{
//    unsigned int array = 0;
//    for (int i = 0; i < length; i++)
//    {
//        array |= start[i] << i;
//    }
//
//    return array;
//}

//unsigned char GetByte(unsigned int number, int index)
//{
//    return ((1 << index) & number) >> index;
//}

//void PrintBinary(unsigned x)
//{
//    std::cout << "Binary : ";
//    for (int i = 31; i >= 0; --i)
//    {
//        int mask = 1 << i;
//        int number = (x & mask) >> i;
//        std::cout << number;
//    }
//    std::cout << std::endl;
//}