#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <optional>

enum class TokenType {
    End = 0,
    Endline,
    Colon,
    Integer,
    Float,
    Symbol,
    Invalid
};

struct Token
{
    TokenType type;
    std::string lexeme;
    size_t line;
    size_t col;
    union {
        uint32_t int_value;
        float float_value;
    } as;
};

std::string formatToken(Token &t);

class Scanner
{
public:
    Scanner(std::string src);
    Token next();
    Token peek();
    
private:
    std::string getLexeme();
    void skipWhitespace();
    bool isWhitespace();
    bool isEnd();
    bool isEndline();
    bool isColon();
    bool isSymbol();

    std::string src;
    size_t begin;
    size_t current;
};

#endif
