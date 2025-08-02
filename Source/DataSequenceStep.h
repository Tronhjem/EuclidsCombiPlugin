#pragma once

#include "Types.h"

constexpr int MAX_SUB_DIVISION = 8;

class DataStep
{
    int mLength;
    uChar mData[MAX_SUB_DIVISION];
};
