#include "scanner.h"

using std::string;
using std::vector;

Scanner::Scanner(string src)
    : src(src)
{
    size_t i = 0;
    size_t line = 1;
    std::optional<size_t> token_start = std::nullopt;
    bool found_delim = false;
    while (i < src.size()) {
        if (src[i] == ' ' || src[i] == '\t') {
            eval_token(src, token_start, i);
            continue;
        }
        if (src[i] == '\n') {
            eval_token(src, token_start, i);
            tokens.push_back({TokenType::Endline, "\n", line});
            line++;
            continue;
        }
        if (src[i] == ':') {
            eval_token(src, token_start, i);
            tokens.push_back({TokenType::Colon, ":", line});
            continue;
        }

        if (!token_start)
            token_start = i;
        i++;
    }

    eval_token(src, token_start, i);
    tokens.push_back({TokenType::End, "", line});

    it = tokens.cbegin();

    /*
    size_t offset = 0;
    size_t end;
    size_t line = 1;
    while ((end = src.find_first_of(" :\n", offset)) != string::npos) {
        printf("%u %u\n", offset, end);
        if (end - offset > 1) {
            string lexeme = src.substr(offset, end - offset);
            tokens.push_back({TokenType::RuleName, lexeme, line});
        }

        if (src[end] == '\n') {
            tokens.push_back({TokenType::Endline, "\n", line});
            line++;
        }
        else if (src[end] == ':') {
            tokens.push_back({TokenType::Colon, ":", line});
        }

        if (end == offset)
            offset++;
        else
            offset = end;
    }
    printf("%u %u\n", offset, end);

    if (offset < src.size()) {
        string lexeme = src.substr(offset, end - offset);
        tokens.push_back({TokenType::Command, lexeme, line});
    }

    tokens.push_back({TokenType::End, "", line});

    it = tokens.cbegin();
    */
}

void Scanner::eval_token(std::string &src, std::optional<size_t> &token_start, size_t &current)
{
    if (token_start) {
        TokenType type;
        std::string lexeme = src.substr(*token_start, current);
        if (lexeme == "box") {
            type = TokenType::Command;
        }
        else {
            type = TokenType::RuleName;
        }
        tokens.push_back({type, lexeme, 0});
        token_start = std::nullopt;
    }
    current++;
}

std::optional<Token> Scanner::next()
{
    if (it == tokens.cend())
        return std::nullopt;
    return std::make_optional<Token>(*it++);
}

std::optional<Token> Scanner::peek()
{
    if ((it + 1) == tokens.cend())
        return std::nullopt;
    return std::make_optional<Token>(*(it + 1));
}
