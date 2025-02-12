#include <cstdlib>
#include <numeric>
#include <format>
#include <iostream>
#include <print>
#include <cstring>

#include "parser.hpp"
#include "opcodes.hpp"

void parseError(Token &t, std::string msg)
{
    std::print(std::cerr, "ERROR (line {} col {}): {}\n", t.line, t.col, msg);
    exit(1);
}

float parseFloat(Scanner &scanner)
{
    Token t = scanner.next();
    if (t.type != TokenType::Float) {
        parseError(t, std::format("expected float; found '{}'", t.lexeme));
    }
    return t.as.float_value;
}

void writeOpCode(Rule &rule, OpCode opcode)
{
    rule.bytecode.push_back(static_cast<uint8_t>(opcode));
}

void writeInt(Rule &rule, uint8_t value)
{
    rule.bytecode.push_back(value);
}

void writeFloat(Rule &rule, float value)
{
    uint8_t temp[4];
    memcpy(temp, &value, sizeof(float));
    rule.bytecode.push_back(temp[0]);
    rule.bytecode.push_back(temp[1]);
    rule.bytecode.push_back(temp[2]);
    rule.bytecode.push_back(temp[3]);
}

std::optional<size_t> findRule(std::vector<Rule> &rules, std::string name)
{
    size_t index = 0;
    for (auto &r: rules) {
        if (r.name == name)
            return index;
        index++;
    }
    return std::nullopt;
}

void parseRandomRuleCall(Scanner &scanner, std::vector<Rule> &rules, size_t ruleIndex)
{
    std::vector<size_t> ruleIndices;
    std::vector<uint32_t> ruleWeights;

    bool prevTokenSymbol = false;
    while (1) {
        Token t = scanner.next();
        if (t.type == TokenType::RightParen)
            break;

        if (t.type == TokenType::Symbol) {
            auto ruleIndex = findRule(rules, t.lexeme);
            if (!ruleIndex)
                parseError(t, std::format("undefined rule: {}", t.lexeme));
            if (ruleIndices.size() == 255)
                parseError(t, std::format("max number of random rules reached (255)"));
            ruleIndices.push_back(*ruleIndex);
            ruleWeights.push_back(1);
            prevTokenSymbol = true;
        }
        else if (t.type == TokenType::Integer && prevTokenSymbol) {
            ruleWeights[ruleWeights.size() - 1] = t.as.int_value;
            prevTokenSymbol = false;
        }
        else {
            parseError(t, std::format("expected rule name"));
        }
    }

    if (ruleIndices.size() == 0) {
        return;
    }

    if (ruleIndices.size() == 1) {
        writeOpCode(rules[ruleIndex], OpCode::callRule);
        writeInt(rules[ruleIndex], ruleIndices[0]);
        return;
    }

    // Calculate probabilty density function values for rules
    float p = 0.0;
    float totalWeights = static_cast<float>(std::accumulate(ruleWeights.begin(), ruleWeights.end(), 0));
    std::vector<float> pdf;
    for (auto w: ruleWeights) {
        p += w / totalWeights;
        pdf.push_back(p);
    }

    writeOpCode(rules[ruleIndex], OpCode::callRandomRule);
    writeInt(rules[ruleIndex], static_cast<uint8_t>(ruleIndices.size()));
    for (auto i = 0; i < ruleIndices.size(); i++) {
        writeInt(rules[ruleIndex], ruleIndices[i]);
        writeFloat(rules[ruleIndex], pdf[i]);
    }
}

void parseRuleAttributes(Scanner &scanner, std::vector<Rule> &rules, size_t ruleIndex)
{
    while (true) {
        Token t = scanner.next();
        if (t.type == TokenType::End || t.type == TokenType::Colon) {
            return;
        }

        if (t.type == TokenType::Symbol && t.lexeme == "maxdepth") {
            t = scanner.next();
            if (t.type != TokenType::Integer)
                parseError(t, "integer value required for maxdepth attribute");
            rules[ruleIndex].maxDepth = t.as.int_value;
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "nextrule") {
            t = scanner.next();
            if (t.type != TokenType::Symbol)
                parseError(t, "valid rule name required for nextrule attribute");
            auto nextRuleIndex = findRule(rules, t.lexeme);
            if (!nextRuleIndex)
                parseError(t, std::format("undefined rule: {}", t.lexeme));
            rules[ruleIndex].nextRuleIndex = nextRuleIndex;
        }
        else {
            parseError(t, std::format("invalid attribute {}", t.lexeme));
        }
    }
}

void parseRuleDef(Scanner &scanner, std::vector<Rule> &rules, size_t ruleIndex)
{
    while (true) {
        Token t = scanner.next();
        if (t.type == TokenType::End || t.type == TokenType::Endline)
            break;

        if (t.type == TokenType::Symbol && t.lexeme == "tx") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::translateX);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "ty") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::translateY);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "tz") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::translateZ);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "rx") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::rotateX);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "ry") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::rotateY);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "rz") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::rotateZ);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "s") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::scale);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "sx") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::scaleX);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "sy") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::scaleY);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "sz") {
            float delta = parseFloat(scanner);
            writeOpCode(rules[ruleIndex], OpCode::scaleZ);
            writeFloat(rules[ruleIndex], delta);
        }
        else if (t.type == TokenType::Symbol && t.lexeme == "box") {
            writeOpCode(rules[ruleIndex], OpCode::drawBox);
        }
        else if (t.type == TokenType::Symbol) {
            writeOpCode(rules[ruleIndex], OpCode::callRule);
            auto nextRule = findRule(rules, t.lexeme);
            if (!nextRule)
                parseError(t, std::format("undefined rule {}", t.lexeme));
            writeInt(rules[ruleIndex], *nextRule);
        }
        else if (t.type == TokenType::LeftParen) {
            parseRandomRuleCall(scanner, rules, ruleIndex);
        }
        else {
            parseError(t, std::format("unexpected symbol '{}'", t.lexeme));
        }
    }
}

bool isEnd(Token &t)
{
    return t.type == TokenType::End;
}

bool isEndLine(Token &t)
{
    return t.type == TokenType::Endline;
}

bool isSymbol(Token &t)
{
    return t.type == TokenType::Symbol;
}

bool isColon(Token &t)
{
    return t.type == TokenType::Colon;
}

void parse(Scanner &scanner, std::vector<Rule> &rules)
{
    // Build rules table
    while (true) {
        Token t = scanner.next();
        if (isEnd(t))
            break;
        if (isEndLine(t))
            continue;

        if (!isSymbol(t))
            parseError(t, std::format("expected rule name, found '{}'", t.lexeme));
        std::string ruleName = t.lexeme;

        if (findRule(rules, ruleName))
            parseError(t, std::format("duplicate rule name '{}'", ruleName));
        rules.push_back({ruleName, {}, 0, 100, std::nullopt});

        while (true) {
            t = scanner.next();
            if (isEnd(t) || isEndLine(t))
                break;
        }
    }

    scanner.reset();

    // Find rule defs and parse them
    while (true) {
        Token t = scanner.next();
        if (t.type == TokenType::End)
            break;
        if (t.type == TokenType::Endline)
            continue;
        if (t.type != TokenType::Symbol)
            parseError(t, std::format("unexpected token '{}'", t.lexeme));
        std::string ruleName = t.lexeme;

        auto foundRuleIndex = findRule(rules, ruleName);
        if (!foundRuleIndex)
            parseError(t, std::format("undefined rule '{}'", ruleName));

        parseRuleAttributes(scanner, rules, *foundRuleIndex);
        parseRuleDef(scanner, rules, *foundRuleIndex);
    }
    writeOpCode(rules[0], OpCode::exit);
}
