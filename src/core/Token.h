#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    ReservedWord,
    IdentifierCode,
    DateLiteral,
    TimeLiteral,
    IntegerLiteral,
    StringLiteral,
    SpecialtyEnum,
    DoseEnum,
    BloodTypeLiteral,
    Delimiter,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

std::string tokenTypeToString(TokenType type);

#endif
