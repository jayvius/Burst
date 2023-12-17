#include "rules.h"

size_t addRule(Rules &rules, std::string name, size_t maxDepth)
{
    rules.ruleTable.push_back({name, {}, 0, maxDepth});
    return rules.ruleTable.size() - 1;
}

std::optional<size_t> getRuleIndex(Rules &rules, std::string name)
{
    for (size_t i = 0; i < rules.ruleTable.size(); i++) {
        if (rules.ruleTable[i].name == name)
            return i;
    }
    return std::nullopt;
}

void emitOpCode(Rules &rules, size_t ruleIndex, OpCode opcode)
{
    rules.ruleTable[ruleIndex].byteCode.push_back(static_cast<uint8_t>(opcode));
}

void emitInt(Rules &rules, size_t ruleIndex, uint8_t value)
{
    rules.ruleTable[ruleIndex].byteCode.push_back(value);
}

void emitFloat(Rules &rules, size_t ruleIndex, float value)
{
    uint8_t temp[4];
    memcpy(temp, &value, sizeof(float));
    rules.ruleTable[ruleIndex].byteCode.push_back(temp[0]);
    rules.ruleTable[ruleIndex].byteCode.push_back(temp[1]);
    rules.ruleTable[ruleIndex].byteCode.push_back(temp[2]);
    rules.ruleTable[ruleIndex].byteCode.push_back(temp[3]);
}
