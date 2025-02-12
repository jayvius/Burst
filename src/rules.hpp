#pragma once

#include <vector>
#include <string>
#include <optional>

#include "opcodes.hpp"

struct Rule
{
    std::string name;
    std::vector<uint8_t> bytecode;
    size_t currentDepth;
    size_t maxDepth;
    std::optional<size_t> nextRuleIndex;
};
