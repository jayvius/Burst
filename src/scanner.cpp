#include "scanner.h"
#include <fmt/core.h>

using std::string;

std::optional<uint32_t> stringToUInt32(const std::string& str)
{
    size_t count;
    unsigned long value;
    try {
        value = std::stoul(str, &count);
    } catch (const std::invalid_argument& e) {
        return std::nullopt;
    } catch (const std::out_of_range& e) {
        return std::nullopt;
    }

    if (count < str.size())
        return std::nullopt;
        
    if (value > std::numeric_limits<std::uint32_t>::max())
        return std::nullopt;
        
    return static_cast<std::uint32_t>(value);
}

std::optional<float> stringToFloat(const std::string& str)
{
    size_t count;
    float value;
    try {
        value = std::stof(str, &count);
    } catch (const std::invalid_argument& e) {
        return std::nullopt;
    } catch (const std::out_of_range& e) {
        return std::nullopt;
    }

    if (count < str.size())
        return std::nullopt;
        
    return static_cast<float>(value);
}

Scanner::Scanner(string src)
    : src(src), begin(0), current(0), lineNum(1)
{

}

Token Scanner::next()
{
    begin = current;

    // Look for beginning of next token or endline
    skipWhitespace();

    if (isEnd())
        return {TokenType::End, "", lineNum, begin+1};

    if (isEndline()) {
        // Skip blank lines
        while (isEndline()) {
            current++;
            skipWhitespace();
            lineNum++;
        }
        return {TokenType::Endline, "", lineNum, begin+1};
    }
    
    // Handle single char tokens
    if (isColon()) {
        current++;
        return {TokenType::Colon, ":", lineNum, begin+1};
    }
    if (isLeftParen()) {
        current++;
        return {TokenType::LeftParen, "(", lineNum, begin+1};
    }
    if (isRightParen()) {
        current++;
        return {TokenType::RightParen, ")", lineNum, begin+1};
    }
    if (isOrOperator()) {
        current++;
        return {TokenType::OrOperator, "|", lineNum, begin+1};
    }

    // Look for end of current token
    while (!isEnd() && !isEndline() && !isWhitespace()
           && !isColon() && !isLeftParen() && !isRightParen() && !isOrOperator())
        current++;

    // Handle symbol or value tokens
    if (std::optional<uint32_t> int_value = stringToUInt32(getLexeme()))
        return {TokenType::Integer, getLexeme(), lineNum, begin+1, .as.int_value=*int_value};
    if (std::optional<float> float_value = stringToFloat(getLexeme()))
        return {TokenType::Float, getLexeme(), lineNum, begin+1, .as.float_value=*float_value};
    if (isSymbol())
        return {TokenType::Symbol, getLexeme(), lineNum, begin+1};

    fmt::print("ERROR (line {} col {}): invalid token {}\n", lineNum, begin+1, getLexeme());
    exit(1);
}

void Scanner::reset()
{
    begin = 0;
    current = 0;
    lineNum = 0;
}

std::string Scanner::getLexeme()
{
    return src.substr(begin, current-begin);
}

void Scanner::skipWhitespace()
{
    while (1) {
        if (current == src.size())
            return;
        char c = src[current];
        if (c != ' ' && c != '\t')
            return;
        current++;
        begin++;
    }
}

bool Scanner::isWhitespace()
{
    return src[current] == ' ' || src[current] == '\t';
}

bool Scanner::isEnd()
{
    return current >= src.size();
}

bool Scanner::isEndline()
{
    return src[current] == '\n';
}

bool Scanner::isColon()
{
    return src[current] == ':';
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z');
}

bool Scanner::isLeftParen()
{
    return src[current] == '(';
}

bool Scanner::isRightParen()
{
    return src[current] == ')';
}

bool Scanner::isOrOperator()
{
    return src[current] == '|';
}

bool Scanner::isSymbol()
{
    size_t i = begin;

    if (!isAlpha(src[i])) return false;

    while (++i < current) {
        if (!isAlpha(src[i]) && !isDigit(src[i]) && src[i] != '_') return false;
    }
    return true;
}

std::string formatToken(Token &t)
{
    if (t.type == TokenType::Endline)
        return fmt::format("type={} lexeme={}", "Endline", t.lexeme);
    if (t.type == TokenType::Colon)
        return fmt::format("type={} lexeme={}", "Colon", t.lexeme);
    if (t.type == TokenType::Symbol)
        return fmt::format("type={} lexeme={}", "Symbol", t.lexeme);
    if (t.type == TokenType::Integer)
        return fmt::format("type={} lexeme={}", "Integer", t.lexeme);
    if (t.type == TokenType::Float)
        return fmt::format("type={} lexeme={}", "Float", t.lexeme);
    if (t.type == TokenType::LeftParen)
        return fmt::format("type={} lexeme={}", "LeftParen", t.lexeme);
    if (t.type == TokenType::RightParen)
        return fmt::format("type={} lexeme={}", "RightParen", t.lexeme);
    if (t.type == TokenType::OrOperator)
        return fmt::format("type={} lexeme={}", "OrOperator", t.lexeme);
    int token_value = static_cast<std::underlying_type<TokenType>::type>(t.type);
    return fmt::format("invalid token type {}", token_value);
}

// int main(int argc, char *argv[])
// {
//     Scanner s("R1\nR1: box");
    
//     while (true) {
//         Token t = s.next();
//         if (t.type == TokenType::End)
//             break;
//         fmt::print("{}\n", formatToken(t));
//     }   
    
//     return 0;
// }
