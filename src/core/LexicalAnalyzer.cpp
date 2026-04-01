#include "LexicalAnalyzer.h"
#include <cctype>

LexicalAnalyzer::LexicalAnalyzer(std::string source)
    : source_(std::move(source)), index_(0), line_(1), column_(1) {}

Token LexicalAnalyzer::nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return Token{TokenType::EndOfFile, "", line_, column_};
    }

    int startLine = line_;
    int startColumn = column_;
    char ch = currentChar();

    // =========================
    // DELIMITADORES
    // =========================
    if (ch == '{' || ch == '}' || ch == '[' || ch == ']' ||
        ch == ':' || ch == ',' || ch == ';') {

        std::string lexeme(1, ch);
        advance();
        return Token{TokenType::Delimiter, lexeme, startLine, startColumn};
    }

    // =========================
    // STRINGS "texto"
    // =========================
    if (ch == '"') {
        std::string lexeme = "";
        advance(); // saltar "

        while (!isAtEnd() && currentChar() != '"') {
            lexeme += currentChar();
            advance();
        }

        if (isAtEnd()) {
            errorManager_.addError(
                lexeme,
                "Cadena sin cerrar",
                "No se encontró cierre de comillas",
                startLine,
                startColumn,
                "CRITICO"
            );
            return Token{TokenType::Unknown, lexeme, startLine, startColumn};
        }

        advance(); // cerrar "
        return Token{TokenType::StringLiteral, lexeme, startLine, startColumn};
    }

    // =========================
    // NÚMEROS / FECHA / HORA
    // =========================
    if (isdigit(ch)) {
        std::string lexeme = "";

        while (isdigit(currentChar()) || currentChar() == '-' || currentChar() == ':') {
            lexeme += currentChar();
            advance();
        }

        // FECHA (YYYY-MM-DD)
        if (lexeme.size() == 10 && lexeme[4] == '-' && lexeme[7] == '-') {
            return Token{TokenType::DateLiteral, lexeme, startLine, startColumn};
        }

        // HORA (HH:MM)
        if (lexeme.size() == 5 && lexeme[2] == ':') {
            return Token{TokenType::TimeLiteral, lexeme, startLine, startColumn};
        }

        // ENTERO
        return Token{TokenType::IntegerLiteral, lexeme, startLine, startColumn};
    }

    // =========================
    // IDENTIFICADORES / PALABRAS
    // =========================
    if (isalpha(ch)) {
        std::string lexeme = "";

        while (isalnum(currentChar()) || currentChar() == '_') {
            lexeme += currentChar();
            advance();
        }

        // PALABRAS RESERVADAS
        if (lexeme == "HOSPITAL" || lexeme == "PACIENTES" ||
            lexeme == "MEDICOS" || lexeme == "CITAS" ||
            lexeme == "DIAGNOSTICOS" || lexeme == "paciente" ||
            lexeme == "medico" || lexeme == "cita" ||
            lexeme == "diagnostico") {

            return Token{TokenType::ReservedWord, lexeme, startLine, startColumn};
        }

        // ESPECIALIDADES
        if (lexeme == "CARDIOLOGIA" || lexeme == "NEUROLOGIA" ||
            lexeme == "PEDIATRIA" || lexeme == "CIRUGIA" ||
            lexeme == "MEDICINA_GENERAL" || lexeme == "ONCOLOGIA") {

            return Token{TokenType::SpecialtyEnum, lexeme, startLine, startColumn};
        }

        // DOSIS
        if (lexeme == "DIARIA" || lexeme == "CADA_8_HORAS" ||
            lexeme == "CADA_12_HORAS" || lexeme == "SEMANAL") {

            return Token{TokenType::DoseEnum, lexeme, startLine, startColumn};
        }

        return Token{TokenType::Unknown, lexeme, startLine, startColumn};
    }

    // =========================
    // CÓDIGOS TIPO MED-001
    // =========================
    if (isalpha(ch) && isalpha(peekChar()) && isalpha(peekChar(2)) && peekChar(3) == '-') {
        std::string lexeme = "";

        while (isalnum(currentChar()) || currentChar() == '-') {
            lexeme += currentChar();
            advance();
        }

        return Token{TokenType::IdentifierCode, lexeme, startLine, startColumn};
    }

    // =========================
    // ERROR
    // =========================
    std::string lexeme(1, ch);

    errorManager_.addError(
        lexeme,
        "Token invalido",
        "Caracter no reconocido",
        startLine,
        startColumn,
        "ERROR"
    );

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
