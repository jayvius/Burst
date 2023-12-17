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

void parseCommand(Scanner &scanner, Token &t, Rules &rules, size_t ruleIndex)
{
    if (t.lexeme == "box")
        emitOpCode(rules, ruleIndex, OpCode::drawBox);
    else if (t.lexeme == "tx") {
        emitOpCode(rules, ruleIndex, OpCode::translateX);
        std::optional<Token> t2 = scanner.next();
        if (!t2 || t2->type != TokenType::Float)
            parseError(t, "expected float argument for 'tx' command ");
        float arg = stof(t2->lexeme);
        emitFloat(rules, ruleIndex, arg);
    }
    else
        parseError(t, "invalid command " + t.lexeme);
}

void parse(Scanner &scanner, Rules &rules)
{
    size_t currentRuleIndex = addRule(rules, "", 0);

    while (std::optional<Token> t = scanner.next()) {
        if (!t || t->type == TokenType::End)
            return;
        else if (t->type == TokenType::Command)
            parseCommand(scanner, *t, rules, currentRuleIndex);
        else
            parseError(*t, "invalid token " + t->lexeme);
    }
}
