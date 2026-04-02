#ifndef MEDLANGPARSER_H
#define MEDLANGPARSER_H

#include <vector>
#include "Token.h"
#include "models/Patient.h"
#include "models/Doctor.h"
#include "models/Appointment.h"

class MedLangParser {
public:
    MedLangParser(const std::vector<Token>& tokens);

    void parse();

    const std::vector<Patient>& getPatients() const;
    const std::vector<Doctor>& getDoctors() const;
    const std::vector<Appointment>& getAppointments() const;

private:
    std::vector<Token> tokens_;
    int current_;

    std::vector<Patient> patients_;
    std::vector<Doctor> doctors_;
    std::vector<Appointment> appointments_;

    bool match(const std::string& lexeme);
    Token advance();
    Token peek();
};

#endif