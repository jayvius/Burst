#include <format>
#include <cstdlib>
#include <cstring>

#include "vm.hpp"
#include "parser.hpp"
#include "cube.hpp"

struct Frame
{
    size_t ruleIndex;
    size_t bytecodeIndex;
    glm::mat4 transformation;
    glm::vec<4, unsigned char> rgba;
};

void printFrame(Frame &f) {
    printf("Frame {ruleIndex=%lu bytecodeIndex=%lu}\n", f.ruleIndex, f.bytecodeIndex);
}

OpCode readOpCode(const std::vector<Rule> &rules, Frame &frame)
{
    return OpCode(rules[frame.ruleIndex].bytecode[frame.bytecodeIndex++]);
}

float readFloat(const std::vector<Rule> &rules, Frame &frame)
{
    float temp;
    memcpy(&temp, &rules[frame.ruleIndex].bytecode[frame.bytecodeIndex], sizeof(float));
    frame.bytecodeIndex += sizeof(float);
    return temp;
}

uint8_t readInt(const std::vector<Rule> &rules, Frame &frame)
{
    uint8_t temp;
    memcpy(&temp, &rules[frame.ruleIndex].bytecode[frame.bytecodeIndex], sizeof(uint8_t));
    frame.bytecodeIndex += sizeof(uint8_t);
    return temp;
}

void drawBox(Buffer &buffer, glm::mat4 &transformation, glm::vec<4, unsigned char> &rgba)
{
    addCube(buffer, transformation, rgba);
    buffer.numObjects++;
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
    transformation = glm::scale(transformation, glm::vec3(delta, 1.0, 1.0));
}

void scaleY(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(1.0, delta, 1.0));
}

void scaleZ(glm::mat4 &transformation, float delta)
{
    transformation = glm::scale(transformation, glm::vec3(1.0, 1.0, delta));
}

void runtimeError(std::string error)
{
    printf("runtime error: %s\n", error.c_str());
    exit(1);
}

void run(std::string src, Buffer &buffer)
{
    std::vector<Rule> rules;

    Scanner scanner(src);
    parse(scanner, rules);

    size_t ruleIndex = 0;
    size_t bytecodeIndex = 0;
    glm::mat4 transformation = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    Frame frame = {ruleIndex, bytecodeIndex, transformation, {255, 255, 255, 255}};
    std::vector<Frame> frameStack;

    while (true) {
        while (frame.bytecodeIndex >= rules[frame.ruleIndex].bytecode.size()) {
            if (frameStack.empty())
                return;
            
            if (rules[frame.ruleIndex].currentDepth > 0) {
                rules[frame.ruleIndex].currentDepth--;
            }
            frame = frameStack.back();
            frameStack.pop_back();
        }

        OpCode opcode = readOpCode(rules, frame);
        if (opcode == OpCode::drawBox) {
            drawBox(buffer, frame.transformation, frame.rgba);
        }
        else if (opcode == OpCode::translateX) {
            float delta = readFloat(rules, frame);
            translateX(frame.transformation, delta);
        }
        else if (opcode == OpCode::translateY) {
            float delta = readFloat(rules, frame);
            translateY(frame.transformation, delta);
        }
        else if (opcode == OpCode::translateZ) {
            float delta = readFloat(rules, frame);
            translateZ(frame.transformation, delta);
        }
        else if (opcode == OpCode::rotateX) {
            float delta = readFloat(rules, frame);
            rotateX(frame.transformation, delta);
        }
        else if (opcode == OpCode::rotateY) {
            float delta = readFloat(rules, frame);
            rotateY(frame.transformation, delta);
        }
        else if (opcode == OpCode::rotateZ) {
            float delta = readFloat(rules, frame);
            rotateZ(frame.transformation, delta);
        }
        else if (opcode == OpCode::scale) {
            float delta = readFloat(rules, frame);
            scale(frame.transformation, delta);
        }
        else if (opcode == OpCode::scaleX) {
            float delta = readFloat(rules, frame);
            scaleX(frame.transformation, delta);
        }
        else if (opcode == OpCode::scaleY) {
            float delta = readFloat(rules, frame);
            scaleY(frame.transformation, delta);
        }
        else if (opcode == OpCode::scaleZ) {
            float delta = readFloat(rules, frame);
            scaleZ(frame.transformation, delta);
        }
        else if (opcode == OpCode::callRule) {
            uint8_t nextRuleIndex = readInt(rules, frame);
            while (rules[nextRuleIndex].currentDepth == rules[nextRuleIndex].maxDepth && rules[nextRuleIndex].nextRuleIndex) {
                rules[nextRuleIndex].currentDepth = 1;
                nextRuleIndex = *rules[nextRuleIndex].nextRuleIndex;
            }
            if (rules[nextRuleIndex].currentDepth == rules[nextRuleIndex].maxDepth)
                continue;
            frameStack.push_back(frame);
            frame.ruleIndex = nextRuleIndex;
            frame.bytecodeIndex = 0;
            rules[frame.ruleIndex].currentDepth++;
        }
        else if (opcode == OpCode::callRandomRule) {
            uint8_t numRules = readInt(rules, frame);
            std::vector<size_t> ruleSet;
            std::vector<float> pdf;
            for (auto i = 0; i < numRules; i++) {
                ruleSet.push_back(readInt(rules, frame));
                pdf.push_back(readFloat(rules, frame));
            }
            float r = std::rand() / static_cast<float>(RAND_MAX);
            size_t i = 0;
            for (auto p: pdf) {
                if (r < p)
                    break;
                i++;
            }
            size_t nextRuleIndex = ruleSet[i];

            while (rules[nextRuleIndex].currentDepth == rules[nextRuleIndex].maxDepth && rules[nextRuleIndex].nextRuleIndex) {
                rules[nextRuleIndex].currentDepth = 1;
                nextRuleIndex = *rules[nextRuleIndex].nextRuleIndex;
            }
            if (rules[nextRuleIndex].currentDepth == rules[nextRuleIndex].maxDepth)
                continue;

            frameStack.push_back(frame);
            frame.ruleIndex = nextRuleIndex;
            frame.bytecodeIndex = 0;
            rules[frame.ruleIndex].currentDepth++;
        }
        else if (opcode == OpCode::exit) {
            continue;
        }
        else {
            runtimeError(std::format("invalid opcode: {}", static_cast<int>(opcode)));
        }
    }

    return;
}
