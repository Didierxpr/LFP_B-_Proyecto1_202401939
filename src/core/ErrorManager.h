#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <string>
#include <vector>

struct LexicalError {
    int number;
    std::string lexeme;
    std::string type;
    std::string description;
    int line;
    int column;
    std::string severity;
};

class ErrorManager {
public:
    void addError(const std::string &lexeme,
                  const std::string &type,
                  const std::string &description,
                  int line,
                  int column,
                  const std::string &severity = "ERROR");

    const std::vector<LexicalError> &getErrors() const;
    void clear();

private:
    std::vector<LexicalError> errors_;
};

#endif
