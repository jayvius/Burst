#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    End = 0,
    Comment,
    Endline,
    Colon,
    RuleName,
    Integer,
    Float,
    Command,
};

struct Token
{
    TokenType type;
    std::string lexeme;
    size_t line;
};

class Scanner
{
public:
    Scanner(std::string src);
    std::optional<Token> next();
    std::optional<Token> peek();
    
private:
    void eval_token(std::string &src, std::optional<size_t> &token_start, size_t current);

    std::string src;
    std::vector<Token> tokens;
    std::vector<Token>::const_iterator it;
};


#endif
