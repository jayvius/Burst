#include "vm.hpp"
#include "parser.h"
#include "cube.hpp"
#include <fmt/core.h>
#include <cstdlib>

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

void translateX(glm::mat4 &transformation, float delta)
{
    transformation = glm::translate(transformation, glm::vec3(delta, 0.0, 0.0));
}

void translateY(glm::mat4 &transformation, float delta)
{
    transformation = glm::translate(transformation, glm::vec3(0.0, delta, 0.0));
}

void translateZ(glm::mat4 &transformation, float delta)
{
    transformation = glm::translate(transformation, glm::vec3(0.0, 0.0, delta));
}

void rotateX(glm::mat4 &transformation, float delta)
{
    transformation = glm::rotate(transformation, glm::radians(delta), glm::vec3(1.0, 0.0, 0.0));
}

void rotateY(glm::mat4 &transformation, float delta)
{
    transformation = glm::rotate(transformation, glm::radians(delta), glm::vec3(0.0, 1.0, 0.0));
}

void rotateZ(glm::mat4 &transformation, float delta)
{
    transformation = glm::rotate(transformation, glm::radians(delta), glm::vec3(0.0, 0.0, 1.0));
}

void scale(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(delta, delta, delta));
}

void scaleX(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(delta, 0.0, 0.0));
}

void scaleY(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(0.0, delta, 0.0));
}

void scaleZ(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(0.0, 0.0, delta));
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
        while (bytecodeIndex >= rules[ruleIndex].bytecode.size()) {
            if (ruleIndexStack.empty() || bytecodeIndexStack.empty())
                return;
            
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
        else if (opcode == OpCode::translateX) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            translateX(transformation, delta);
        }
        else if (opcode == OpCode::translateY) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            translateY(transformation, delta);
        }
        else if (opcode == OpCode::translateZ) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            translateZ(transformation, delta);
        }
        else if (opcode == OpCode::rotateX) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            rotateX(transformation, delta);
        }
        else if (opcode == OpCode::rotateY) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            rotateY(transformation, delta);
        }
        else if (opcode == OpCode::rotateZ) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            rotateZ(transformation, delta);
        }
        else if (opcode == OpCode::scale) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            scale(transformation, delta);
        }
        else if (opcode == OpCode::scaleX) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            scaleX(transformation, delta);
        }
        else if (opcode == OpCode::scaleY) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            scaleY(transformation, delta);
        }
        else if (opcode == OpCode::scaleZ) {
            float delta = readFloat(rules[ruleIndex], bytecodeIndex);
            scaleZ(transformation, delta);
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
        else if (opcode == OpCode::callRandomRule) {
            uint8_t numRules = readInt(rules[ruleIndex], bytecodeIndex);
            std::vector<size_t> ruleSet;
            for (auto i = 0; i < numRules; i++)
                ruleSet.push_back(readInt(rules[ruleIndex], bytecodeIndex));
            size_t nextRuleIndex = ruleSet[std::rand() % ruleSet.size()];

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
            runtimeError(fmt::format("invalid opcode: {}", static_cast<int>(opcode)));
        }
    }

    return;
}
