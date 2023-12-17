#include "rules.h"

Rule::Rule(std::string name, size_t maxDepth)
    : name(name), currentDepth(0), maxDepth(maxDepth)
{

}

void Rule::writeOpCode(OpCode opcode)
{
    byteCode.push_back(static_cast<uint8_t>(opcode));
}

void Rule::writeInt(uint8_t value)
{
    byteCode.push_back(value);
}

void Rule::writeFloat(float value)
{
    uint8_t temp[4];
    memcpy(temp, &value, sizeof(float));
    byteCode.push_back(temp[0]);
    byteCode.push_back(temp[1]);
    byteCode.push_back(temp[2]);
    byteCode.push_back(temp[3]);
}
