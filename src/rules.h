#ifndef RULES_H
#define RULES_H

#include <vector>
#include <string>

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

#endif
