#include "parser.h"
#include "burst.h"
#include "opcodes.h"
#include <cstdlib>
#include <format>

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
void parseError(Token &t, std::string error)
{
    printf("parse error (line %u): %s\n", t.line, error.c_str());
    exit(1);
}

void parseCommand(Scanner &scanner, Token &t, Rule &rule)
{
    if (t.lexeme == "box")
        writeOpCode(rule, OpCode::drawBox);
    else if (t.lexeme == "tx") {
        writeOpCode(rule, OpCode::translateX);
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'tx' command ");
        float arg = stof(t2->lexeme);
        writeFloat(rule, arg);
    }
    else if (t.lexeme == "ty") {
        writeOpCode(rule, OpCode::translateY);
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'ty' command ");
        float arg = stof(t2->lexeme);
        writeFloat(rule, arg);
    }
    else if (t.lexeme == "tz") {
        writeOpCode(rule, OpCode::translateZ);
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'tz' command ");
        float arg = stof(t2->lexeme);
        writeFloat(rule, arg);
    }
    else
        parseError(t, "invalid command " + t.lexeme);
}

void parseRuleCall(Scanner &scanner, Token &t, std::vector<Rule> &rules, size_t ruleIndex)
{
    printf("parseRuleCall()\n");
    size_t nextRuleIndex = 0;
    for (auto i = 0; i < rules.size(); i++) {
        if (rules[i].name == t.lexeme) {
            nextRuleIndex = i;
            break;
        }
    }
    if (nextRuleIndex == 0)
        parseError(t, "unknown rule " + t.lexeme);
    writeOpCode(rules[ruleIndex], OpCode::callRule);
    writeInt(rules[ruleIndex], static_cast<uint8_t>(nextRuleIndex));
}

void parseRuleDef(Scanner &scanner, Token &t, std::vector<Rule> &rules)
{
    printf("parseRuleDef()\n");
    if (rules.size() == 256)
        parseError(t, "could not add rule " + t.lexeme + "; maximum number of rules reached");
    rules.push_back({t.lexeme, {}, 0, 50});
    size_t ruleIndex = rules.size() - 1;
    // Consume colon
    scanner.next();
    while (std::optional<Token> t = scanner.next()) {
        if (!t || t->type == TokenType::End)
            break;
        else if (t->type == TokenType::Endline)
            break;
        else if (t->type == TokenType::Command)
            parseCommand(scanner, *t, rules[ruleIndex]);
        else if (t->type == TokenType::RuleName)
            parseRuleCall(scanner, *t, rules, ruleIndex);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }

    writeOpCode(rules[ruleIndex], OpCode::exit);
    return;
}

std::vector<Rule> parse(Scanner &scanner)
{
    std::vector<Rule> rules;
    rules.push_back({"", {}, 0, 0});
    size_t ruleIndex = 0;

    while (std::optional<Token> t = scanner.next()) {
        if (!t || t->type == TokenType::End)
            break;
        else if (t->type == TokenType::RuleName && scanner.peek() && scanner.peek()->type == TokenType::Colon)
            parseRuleDef(scanner, *t, rules);
        else if (t->type == TokenType::RuleName)
            parseRuleCall(scanner, *t, rules, ruleIndex);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }
    writeOpCode(rules[0], OpCode::exit);
    return rules;
}
