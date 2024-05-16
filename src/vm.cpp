#include "vm.hpp"
#include "parser.h"
#include "cube.hpp"
#include <fmt/core.h>

OpCode readOpCode(Rule &rule, size_t &bytecodeIndex)
{
    return OpCode(rule.bytecode[bytecodeIndex++]);
}

float readFloat(Rule &rule, size_t &bytecodeIndex)
{
    float temp;
    memcpy(&temp, &rule.bytecode[bytecodeIndex], sizeof(float));
    bytecodeIndex += sizeof(float);
    return temp;
}

uint8_t readInt(Rule &rule, size_t &bytecodeIndex)
{
    uint8_t temp;
    memcpy(&temp, &rule.bytecode[bytecodeIndex], sizeof(uint8_t));
    bytecodeIndex += sizeof(uint8_t);
    return temp;
}

void drawBox(Buffer &buffer, glm::mat4 &transformation)
{
    addCube(buffer, transformation);
}

void rotateY(glm::mat4 &transformation, float delta)
{
    transformation = glm::rotate(transformation, glm::radians(delta), glm::vec3(0.0, 1.0, 0.0));
}

void runtimeError(std::string error)
{
    printf("runtime error: %s\n", error.c_str());
    exit(1);
}

void run(VM &vm, std::string src, Buffer &buffer)
{
    std::vector<Rule> rules;
    size_t ruleIndex = 0;
    size_t bytecodeIndex = 0;
    glm::mat4 transformation = glm::mat4(1.0f);

    std::vector<size_t> ruleIndexStack;
    std::vector<size_t> bytecodeIndexStack;
    std::vector<glm::mat4> transformationStack;

    Scanner scanner(src);
    parse(scanner, rules);

    while (true) {
        if (bytecodeIndex >= rules[ruleIndex].bytecode.size()) {
            if (ruleIndexStack.empty() || bytecodeIndexStack.empty())
                break;
            
            rules[ruleIndex].currentDepth--;
            ruleIndex = ruleIndexStack.back();
            ruleIndexStack.pop_back();
            bytecodeIndex = bytecodeIndexStack.back();
            bytecodeIndexStack.pop_back();
            transformation = transformationStack.back();
            transformationStack.pop_back();
        }

        OpCode opcode = readOpCode(rules[ruleIndex], bytecodeIndex);
        if (opcode == OpCode::drawBox) {
            drawBox(buffer, transformation);
        }
        else if (opcode == OpCode::rotateY) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            rotateY(transformation, delta);
        }
        else if (opcode == OpCode::callRule) {
            uint8_t nextRuleIndex = readInt(rules[ruleIndex], bytecodeIndex);
            if (rules[nextRuleIndex].currentDepth == rules[nextRuleIndex].maxDepth)
                continue;
            rules[nextRuleIndex].currentDepth++;
            ruleIndexStack.push_back(ruleIndex);
            ruleIndex = nextRuleIndex;
            bytecodeIndexStack.push_back(bytecodeIndex);
            bytecodeIndex = 0;
            transformationStack.push_back(transformation);
        }
        else if (opcode == OpCode::exit) {
            continue;
        }
        else {
            runtimeError("invalid opcode");
        }
    }

    return;
}
