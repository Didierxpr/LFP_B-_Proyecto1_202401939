#include "Token.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ReservedWord:
            return "Palabra Reservada";
        case TokenType::IdentifierCode:
            return "Codigo ID";
        case TokenType::DateLiteral:
            return "Fecha";
        case TokenType::TimeLiteral:
            return "Hora";
        case TokenType::IntegerLiteral:
            return "Entero";
        case TokenType::StringLiteral:
            return "Cadena";
        case TokenType::SpecialtyEnum:
            return "Especialidad";
        case TokenType::DoseEnum:
            return "Dosis";
        case TokenType::BloodTypeLiteral:
            return "Tipo Sangre";
        case TokenType::Delimiter:
            return "Delimitador";
        case TokenType::EndOfFile:
            return "EOF";
        default:
            return "Desconocido";
    }
}
