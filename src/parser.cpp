#include "parser.h"
#include "burst.h"
#include "opcodes.h"
#include <cstdlib>
#include <format>

void parseError(Token &t, std::string error)
{
    printf("parse error (line %u): %s\n", t.line, error.c_str());
    exit(1);
}

void parseCommand(Scanner &scanner, Token &t, Rule &rule)
{
    if (t.lexeme == "box")
        rule.writeOpCode(OpCode::drawBox);
    else if (t.lexeme == "tx") {
        rule.writeOpCode(OpCode::translateX);
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'tx' command ");
        float arg = stof(t2->lexeme);
        rule.writeFloat(arg);
    }
    else
        parseError(t, "invalid command " + t.lexeme);
}

std::vector<Rule> parse(Scanner &scanner)
{
    std::vector<Rule> rules;
    rules.push_back(Rule("", 0));
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
