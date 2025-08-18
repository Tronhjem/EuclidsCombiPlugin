#pragma once

#include <stdint.h>
#include "DataSequenceStep.h"

typedef unsigned char uChar;
//typedef uChar StepData;
typedef DataSequenceStep StepData;

constexpr int MAX_DATASEQUENCE_LENGTH = 32;
constexpr int MAX_UCHAR_VALUE = 127;

//static_assert(sizeof(uChar) == 1);

//#define EXPECTED_LOGIC_SEQUENCE_SIZE 32
//static_assert(MAX_LOGIC_SEQUENCE_LENGTH == EXPECTED_LOGIC_SEQUENCE_SIZE);
