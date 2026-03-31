#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>

#include "ErrorManager.h"
#include "Token.h"

class LexicalAnalyzer {
public:
    explicit LexicalAnalyzer(std::string source);

    Token nextToken();
    std::vector<Token> scanAllTokens();

    void reset(std::string newSource);
    const std::vector<LexicalError> &getErrors() const;

private:
    char currentChar() const;
    char peekChar(int offset = 1) const;
    bool isAtEnd() const;
    void advance();
    void skipWhitespace();

    std::string source_;
    std::size_t index_;
    int line_;
    int column_;
    ErrorManager errorManager_;
};

#endif
