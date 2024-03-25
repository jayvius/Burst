from enum import IntEnum, auto
from dataclasses import dataclass
from typing import List, Optional
import re


class TokenType(IntEnum):
    COLON = auto()
    INT = auto()
    FLOAT = auto()
    SYMBOL = auto()
    ENDLINE = auto()


@dataclass
class Token:
    type: TokenType
    line: int
    col: int
    lexeme: Optional[str] = None


def is_int(lexeme):
    try:
        int(lexeme)
        return True
    except ValueError:
        return False


def is_float(lexeme):
    try:
        float(lexeme)
        return True
    except ValueError:
        return False


def scan(source: str) -> List[Token]:
    tokens = []
    for line in re.split(r'[\n]+', source):
        if not line:
            continue
        for lexeme in re.split(r'\s+|(:)', line):
            if not lexeme:
                continue
            if lexeme == ':':
                tokens.append(Token(type=TokenType.COLON, lexeme=':', line=1, col=1))
            elif is_int(lexeme):
                tokens.append(Token(type=TokenType.INT, lexeme=lexeme, line=1, col=1))
            elif is_float(lexeme):
                tokens.append(Token(type=TokenType.FLOAT, lexeme=lexeme, line=1, col=1))
            else:
                tokens.append(Token(type=TokenType.SYMBOL, lexeme=lexeme, line=1, col=1))
        tokens.append(Token(type=TokenType.ENDLINE, line=1, col=1))
    return tokens


if __name__ == '__main__':
    def test(input: str, expected: List[Token]):
        actual = scan(input)
        if expected == actual:
            print(f'PASS: {input}')
        else:
            print(f'\033[31mFAIL: {input}\033[0m')
            print('  expected:')
            for e in expected:
                print(f'    {e}')
            print('  actual:')
            for a in actual:
                print(f'    {a}')

    test('', [])
    test('R1', [
        Token(type=TokenType.SYMBOL, lexeme='R1', line=1, col=1),
        Token(type=TokenType.ENDLINE, line=1, col=1),
    ])
    test('R1 : box', [
        Token(type=TokenType.SYMBOL, lexeme='R1', line=1, col=1),
        Token(type=TokenType.COLON, lexeme=':', line=1, col=1),
        Token(type=TokenType.SYMBOL, lexeme='box', line=1, col=1),
        Token(type=TokenType.ENDLINE, line=1, col=1),
    ])
    test('R1 : tx 10.0', [
        Token(type=TokenType.SYMBOL, lexeme='R1', line=1, col=1),
        Token(type=TokenType.COLON, lexeme=':', line=1, col=1),
        Token(type=TokenType.SYMBOL, lexeme='tx', line=1, col=1),
        Token(type=TokenType.FLOAT, lexeme='10.0', line=1, col=1),
        Token(type=TokenType.ENDLINE, line=1, col=1),
    ])
