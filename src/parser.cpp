#include "parser.h"
#include "burst.h"
#include "opcodes.h"
#include <cstdlib>
#include <format>

void writeOpCode(Rule &rule, OpCode opcode)
{
    rule.byteCode.push_back(static_cast<uint8_t>(opcode));
}

void writeInt(Rule &rule, uint8_t value)
{
    rule.byteCode.push_back(value);
}

void writeFloat(Rule &rule, float value)
{
    uint8_t temp[4];
    memcpy(temp, &value, sizeof(float));
    rule.byteCode.push_back(temp[0]);
    rule.byteCode.push_back(temp[1]);
    rule.byteCode.push_back(temp[2]);
    rule.byteCode.push_back(temp[3]);
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
    else
        parseError(t, "invalid command " + t.lexeme);
}

std::vector<Rule> parse(Scanner &scanner)
{
    std::vector<Rule> rules;
    rules.push_back({"", {}, 0, 0});
    size_t ruleIndex = 0;

    while (std::optional<Token> t = scanner.next()) {
        if (!t || t->type == TokenType::End)
            return rules;
        else if (t->type == TokenType::Command)
            parseCommand(scanner, *t, rules[ruleIndex]);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }
    return rules;
}
