#include "parser.h"
#include "burst.h"
#include <cstdlib>
#include <format>

std::optional<size_t> getRuleIndex(std::vector<Rule> &table, std::string name)
{
    size_t index = 0;
    for (const auto &rule: table) {
        if (rule.name == name)
            return index;
        index++;
    }
    return std::nullopt;
}

void parseError(Token &t, std::string error)
{
    printf("parse error (line %u): %s\n", t.line, error.c_str());
    exit(1);
}

void parseCommand(Scanner &scanner, Token &t, Rules &rules, size_t index)
{
    if (t.lexeme == "box")
        rules.ruleTable[index].byteCode.push_back(static_cast<uint8_t>(OpCode::drawBox));
    else if (t.lexeme == "tx") {
        rules.ruleTable[index].byteCode.push_back(static_cast<uint8_t>(OpCode::translateX));
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'tx' command ");
        float arg = stof(t2->lexeme);
        uint8_t temp[4];
        memcpy(temp, &arg, sizeof(float));
        rules.ruleTable[index].byteCode.push_back(temp[0]);
        rules.ruleTable[index].byteCode.push_back(temp[1]);
        rules.ruleTable[index].byteCode.push_back(temp[2]);
        rules.ruleTable[index].byteCode.push_back(temp[3]);
    }
    else
        parseError(t, "invalid command " + t.lexeme);
}

void parse(Scanner &scanner, Rules &rules)
{
    // Initialize default rule
    rules.ruleTable.push_back({"", {}, 0, 0});
    size_t currentRule = 0;

    while (std::optional<Token> t = scanner.next()) {
        if (!t || t->type == TokenType::End)
            return;
        else if (t->type == TokenType::Command)
            parseCommand(scanner, *t, rules, currentRule);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }
}
