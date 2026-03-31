#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(std::string source)
    : source_(std::move(source)), index_(0), line_(1), column_(1) {}

Token LexicalAnalyzer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return Token{TokenType::EndOfFile, "", line_, column_};
    }

    const int startLine = line_;
    const int startColumn = column_;
    const char ch = currentChar();

    // Parte 1: esqueleto inicial. El AFD completo se implementa en la Parte 2.
    std::string lexeme(1, ch);
    errorManager_.addError(
        lexeme,
        "Token invalido",
        "Token no reconocido por el analizador en estado base.",
        startLine,
        startColumn,
        "ERROR");

    advance();
    return Token{TokenType::Unknown, lexeme, startLine, startColumn};
}

std::vector<Token> LexicalAnalyzer::scanAllTokens() {
    std::vector<Token> tokens;
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }
    return tokens;
}

void LexicalAnalyzer::reset(std::string newSource) {
    source_ = std::move(newSource);
    index_ = 0;
    line_ = 1;
    column_ = 1;
    errorManager_.clear();
}

const std::vector<LexicalError> &LexicalAnalyzer::getErrors() const {
    return errorManager_.getErrors();
}

char LexicalAnalyzer::currentChar() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source_[index_];
}

char LexicalAnalyzer::peekChar(int offset) const {
    const std::size_t target = index_ + static_cast<std::size_t>(offset);
    if (target >= source_.size()) {
        return '\0';
    }
    return source_[target];
}

bool LexicalAnalyzer::isAtEnd() const {
    return index_ >= source_.size();
}

void LexicalAnalyzer::advance() {
    if (isAtEnd()) {
        return;
    }

    if (source_[index_] == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    ++index_;
}

void LexicalAnalyzer::skipWhitespace() {
    while (!isAtEnd()) {
        const char ch = currentChar();
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            advance();
        } else {
            break;
        }
    }
}
