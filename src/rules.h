#ifndef RULES_H
#define RULES_H

#include <vector>
#include <string>
#include <optional>
#include "opcodes.h"

class Rule
{
public:
    Rule(std::string name, size_t maxDepth);
    void writeOpCode(OpCode opcode);
    void writeFloat(float value);
    void writeInt(uint8_t value);

    std::string name;
    std::vector<uint8_t> byteCode;
    size_t currentDepth;
    size_t maxDepth;
};

struct Rules
{
    std::vector<Rule> ruleTable;
    std::vector<size_t> ruleStack;
    std::vector<size_t> ruleIndexStack;
};

#endif
