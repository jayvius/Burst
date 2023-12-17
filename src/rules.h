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

struct Rules
{
    std::vector<Rule> ruleTable;
    std::vector<size_t> ruleStack;
    std::vector<size_t> ruleIndexStack;
};

size_t addRule(Rules &rules, std::string name, size_t maxDepth);
std::optional<size_t> getRuleIndex(Rules &rules, std::string name);

void emitOpCode(Rules &rules, size_t ruleIndex, OpCode opcode);
void emitInt(Rules &rules, size_t ruleIndex, uint8_t value);
void emitFloat(Rules &rules, size_t ruleIndex, float value);

#endif
