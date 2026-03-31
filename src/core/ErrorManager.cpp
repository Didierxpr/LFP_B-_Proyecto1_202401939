#include "ErrorManager.h"

void ErrorManager::addError(const std::string &lexeme,
                            const std::string &type,
                            const std::string &description,
                            int line,
                            int column,
                            const std::string &severity) {
    LexicalError error{};
    error.number = static_cast<int>(errors_.size()) + 1;
    error.lexeme = lexeme;
    error.type = type;
    error.description = description;
    error.line = line;
    error.column = column;
    error.severity = severity;
    errors_.push_back(error);
}

const std::vector<LexicalError> &ErrorManager::getErrors() const {
    return errors_;
}

void ErrorManager::clear() {
    errors_.clear();
}
