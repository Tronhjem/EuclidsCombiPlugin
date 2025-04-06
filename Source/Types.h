#pragma once

#include <stdint.h>

typedef unsigned char uChar;
typedef int32_t LogicContainer;

static_assert(sizeof(uChar) == 1);
static_assert(sizeof(LogicContainer) == 4);

#define EXPECTED_LOGIC_SEQUENCE_SIZE 32
constexpr int MAX_LOGIC_SEQUENCE_LENGTH = sizeof(LogicContainer) * 8;
static_assert(MAX_LOGIC_SEQUENCE_LENGTH == EXPECTED_LOGIC_SEQUENCE_SIZE);
