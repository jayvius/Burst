#ifndef RULES_H
#define RULES_H

#include <vector>
#include <string>
#include <optional>
#include "opcodes.h"

struct Rule
{
    std::string name;
    std::vector<uint8_t> byteCode;
    size_t currentDepth;
    size_t maxDepth;
};

#endif
