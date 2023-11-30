#include "parser.h"
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
    printf("Parser Error (line %u): %s\n", t.line, error.c_str());
    exit(1);
}

void parseCommand(Scanner &scanner, Token &t, Rules &rules, size_t index)
{
    if (t.lexeme == "box")
        rules.ruleTable[index].byteCode.push_back(4);
    else
        parseError(t, "invalid command " + t.lexeme);
}

void parse(Scanner &scanner, Rules &rules)
{
    // Initialize default rule
    rules.ruleTable.push_back({"", {}, 0, 0});
    size_t currentRule = 0;

    while (std::optional<Token> t = scanner.next()) {
        if (t && t->type == TokenType::Command)
            parseCommand(scanner, *t, rules, currentRule);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }
}
