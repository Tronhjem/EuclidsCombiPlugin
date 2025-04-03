#pragma once

#include <stdint.h>

typedef unsigned char uChar;
typedef int32_t Trigger;

static_assert(sizeof(uChar) == 1);
static_assert(sizeof(Trigger) == 4);

#define EXPECTED_LOGIC_SEQUENCE_SIZE 32
constexpr int MAX_LOGIC_SEQUENCE_LENGTH = sizeof(Trigger) * 8;
static_assert(MAX_LOGIC_SEQUENCE_LENGTH == EXPECTED_LOGIC_SEQUENCE_SIZE);
