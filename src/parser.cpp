#include "parser.h"
#include "opcodes.h"
#include <cstdlib>
#include <fmt/core.h>

#ifdef BURST_DEBUG
constexpr bool debug_mode = true;
#else
constexpr bool debug_mode = false;
#endif

void parseError(Token &t, std::string msg)
{
    fmt::print(stderr, "ERROR (line {} col {}): {}\n", t.line, t.col, msg);
    exit(1);
}

float parseFloat(Scanner &scanner)
{
    Token t = scanner.next();
    if (t.type != TokenType::Float) {
        parseError(t, fmt::format("expected float; found '{}'", t.lexeme));
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

void parseCommand(Scanner &scanner, Token &t, Rule &rule)
{
    // if (t.lexeme == "box")
    //     writeOpCode(rule, OpCode::drawBox);
    // else if (t.lexeme == "tx") {
    //     writeOpCode(rule, OpCode::translateX);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'tx' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "ty") {
    //     writeOpCode(rule, OpCode::translateY);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'ty' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "tz") {
    //     writeOpCode(rule, OpCode::translateZ);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'tz' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "rx") {
    //     writeOpCode(rule, OpCode::rotateX);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'rx' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "ry") {
    //     writeOpCode(rule, OpCode::rotateY);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'ry' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "rz") {
    //     writeOpCode(rule, OpCode::rotateY);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 'rz' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else if (t.lexeme == "s") {
    //     writeOpCode(rule, OpCode::scale);
    //     std::optional<Token> t2 = scanner.next();
    //     if (!t2 || t2->type != TokenType::Float)
    //         parseError(t, "expected float argument for 's' command ");
    //     float arg = stof(t2->lexeme);
    //     writeFloat(rule, arg);
    // }
    // else
    //     parseError(t, "invalid command " + t.lexeme);
}

void parseRuleCall(Scanner &scanner, Token &t, std::vector<Rule> &rules, size_t ruleIndex)
{
    // size_t nextRuleIndex = 0;
    // for (auto i = 0; i < rules.size(); i++) {
    //     if (rules[i].name == t.lexeme) {
    //         nextRuleIndex = i;
    //         break;
    //     }
    // }
    // if (nextRuleIndex == 0) {
    //     rules.push_back({t.lexeme, {}, 0, 50});
    //     nextRuleIndex = rules.size() - 1;
    // }
    // writeOpCode(rules[ruleIndex], OpCode::callRule);
    // writeInt(rules[ruleIndex], static_cast<uint8_t>(nextRuleIndex));
}

void parseRuleDef(Scanner &scanner, Token &t, std::vector<Rule> &rules)
{
    // size_t ruleIndex = 0;
    // for (auto i = 0; i < rules.size(); i++) {
    //     if (rules[i].name == t.lexeme) {
    //         ruleIndex = i;
    //         break;
    //     }
    // }
    // if (ruleIndex == 0 && rules.size() == 256)
    //     parseError(t, "could not add rule " + t.lexeme + "; maximum number of rules reached");
    // if (ruleIndex == 0) {
    //     rules.push_back({t.lexeme, {}, 0, 50});
    //     ruleIndex = rules.size() - 1;
    // }
    // // Consume colon
    // scanner.next();
    // while (std::optional<Token> t = scanner.next()) {
    //     if (!t || t->type == TokenType::End)
    //         break;
    //     else if (t->type == TokenType::Endline)
    //         break;
    //     else if (t->type == TokenType::Command)
    //         parseCommand(scanner, *t, rules[ruleIndex]);
    //     else if (t->type == TokenType::RuleName)
    //         parseRuleCall(scanner, *t, rules, ruleIndex);
    //     else
    //         parseError(*t, "invalid token " + t->lexeme);
    // }

    // writeOpCode(rules[ruleIndex], OpCode::exit);
    // return;
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

    while (1) {
        Token t = scanner.next();
        if (t.type == TokenType::RightParen)
            break;

        if (t.type == TokenType::Symbol) {
            auto ruleIndex = findRule(rules, t.lexeme);
            if (!ruleIndex)
                parseError(t, fmt::format("undefined rule: {}", t.lexeme));
            if (ruleIndices.size() == 255)
                parseError(t, fmt::format("max number of random rules reached (255)"));
            ruleIndices.push_back(*ruleIndex);
        }
        else {
            parseError(t, fmt::format("expected rule name"));
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

    writeOpCode(rules[ruleIndex], OpCode::callRandomRule);
    writeInt(rules[ruleIndex], static_cast<uint8_t>(ruleIndices.size()));
    for (auto i: ruleIndices)
        writeInt(rules[ruleIndex], i);
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
                parseError(t, fmt::format("undefined rule {}", t.lexeme));
            writeInt(rules[ruleIndex], *nextRule);
        }
        else if (t.type == TokenType::LeftParen) {
            parseRandomRuleCall(scanner, rules, ruleIndex);
        }
        else {
            parseError(t, fmt::format("unexpected symbol '{}'", t.lexeme));
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
    //rules.push_back({"", {}, 0, 0});
    // writeOpCode(rules[0], OpCode::rotateY);
    // writeFloat(rules[0], 45.0f);
    // writeOpCode(rules[0], OpCode::drawBox);
    // writeOpCode(rules[0], OpCode::exit);

    // Build rules table
    while (true) {
        Token t = scanner.next();
        if (isEnd(t))
            break;
        if (isEndLine(t))
            continue;

        if (!isSymbol(t))
            continue;
        std::string ruleName = t.lexeme;

        t = scanner.next();
        if (!isColon(t))
            continue;

        if (findRule(rules, ruleName))
            parseError(t, fmt::format("duplicate rule name '{}'", ruleName));
        rules.push_back({ruleName, {}, 0, 100});

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
            parseError(t, fmt::format("unexpected token '{}'", t.lexeme));
        std::string ruleName = t.lexeme;

        t = scanner.next();
        if (t.type != TokenType::Colon)
            parseError(t, "expected rule def");

        auto foundRuleIndex = findRule(rules, ruleName);
        if (!foundRuleIndex)
            parseError(t, fmt::format("undefined rule '{}'", ruleName));
        parseRuleDef(scanner, rules, *foundRuleIndex);
    }
    writeOpCode(rules[0], OpCode::exit);

    // while (std::optional<Token> t = scanner.next()) {
    //     if (!t || t->type == TokenType::End)
    //         break;
    //     else if (t->type == TokenType::Endline)
    //         continue;
    //     else if (t->type == TokenType::RuleName && scanner.peek() && scanner.peek()->type == TokenType::Colon)
    //         parseRuleDef(scanner, *t, rules);
    //     else if (t->type == TokenType::RuleName)
    //         parseRuleCall(scanner, *t, rules, ruleIndex);
    //     else
    //         parseError(*t, "invalid token " + t->lexeme + "; expected rule definition");
    // }

    // for (auto &r: rules) {
    //     if (r.bytecode.empty()) {
    //         printf("parse error: no definition for rule %s\n", r.name.c_str());
    //         exit(1);
    //     }
    // }
}
