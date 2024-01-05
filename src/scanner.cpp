#include "scanner.h"

using std::string;
using std::vector;

bool isCommand(std::string &lexeme)
{
    return (lexeme == "box" ||
            lexeme == "tx" ||
            lexeme == "ty" ||
            lexeme == "tz");
}

bool isInt(std::string &lexeme)
{
    size_t i = 0;
    if (lexeme[i] == '+' || lexeme[i] == '-')
        i++;
    while (i < lexeme.size() && isdigit(lexeme[i]))
        i++;
    return i == lexeme.size();
}

bool isFloat(std::string &lexeme)
{
    size_t i = 0;
    if (lexeme[i] == '+' || lexeme[i] == '-')
        i++;
    while (i < lexeme.size() && isdigit(lexeme[i]))
        i++;
    if (i == lexeme.size())
        return false;
    if (lexeme[i] != '.')
        return false;
    i++;
    while (i < lexeme.size() && isdigit(lexeme[i]))
        i++;
    return i == lexeme.size();
}

Scanner::Scanner(string src)
    : src(src)
{
    size_t i = 0;
    size_t line = 1;
    std::optional<size_t> token_start = std::nullopt;
    bool comment = false;
    while (i < src.size()) {
        if (comment) {
            if (src[i] == '\n')
                comment = false;
            i++;
            continue;
        }
        else if (src[i] == ' ' || src[i] == '\t' || src[i] == '\n' || src[i] == ':' || src[i] == '#') {
            eval_token(src, token_start, i, line);
            token_start = std::nullopt;

            if (src[i] == '\n')
                tokens.push_back({TokenType::Endline, "\n", line++});
            else if (src[i] == ':')
                tokens.push_back({TokenType::Colon, ":", line});
            else if (src[i] == '#')
                comment = true;

            i++;
            continue;
        }

        if (!token_start)
            token_start = i;

        i++;
    }

    eval_token(src, token_start, i, line);
    tokens.push_back({TokenType::End, "", line});

    it = tokens.cbegin();
}

void Scanner::eval_token(std::string &src, std::optional<size_t> &token_start, size_t current, size_t line)
{
    if (token_start) {
        TokenType type;
        std::string lexeme = src.substr(*token_start, current - *token_start);
        if (isCommand(lexeme)) {
            type = TokenType::Command;
        }
        else if (isInt(lexeme)) {
            type = TokenType::Integer;
        }
        else if (isFloat(lexeme)) {
            type = TokenType::Float;
        }
        else {
            type = TokenType::RuleName;
        }
        tokens.push_back({type, lexeme, line});
    }
}

std::optional<Token> Scanner::next()
{
    if (it == tokens.cend())
        return std::nullopt;
    return std::make_optional<Token>(*it++);
}

std::optional<Token> Scanner::peek()
{
    if (it == tokens.cend())
        return std::nullopt;
    return std::make_optional<Token>(*it);
}
