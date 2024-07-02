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
    LeftParen,
    RightParen,
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
    void reset();
    
private:
    std::string getLexeme();
    void skipWhitespace();
    bool isWhitespace();
    bool isEnd();
    bool isEndline();
    bool isColon();
    bool isLeftParen();
    bool isRightParen();
    bool isSymbol();
    bool isComment();

    std::string src;
    size_t begin;
    size_t current;
    size_t lineNum;
};

#endif
