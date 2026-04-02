#include "MedLangParser.h"

MedLangParser::MedLangParser(const std::vector<Token>& tokens)
    : tokens_(tokens), current_(0) {}

Token MedLangParser::peek() {
    return tokens_[current_];
}

Token MedLangParser::advance() {
    return tokens_[current_++];
}

bool MedLangParser::match(const std::string& lexeme) {
    if (peek().lexeme == lexeme) {
        advance();
        return true;
    }
    return false;
}

void MedLangParser::parse() {
    while (current_ < tokens_.size()) {
        Token token = peek();
        
        // Buscar PACIENTES
        if (token.lexeme == "PACIENTES") {
            advance(); // consume "PACIENTES"
            advance(); // consume "{"
            
            while (current_ < tokens_.size() && peek().lexeme != "}") {
                if (peek().lexeme == "paciente") {
                    advance(); // consume "paciente"
                    advance(); // consume ":"
                    
                    Patient p;
                    p.name = advance().lexeme; // nombre
                    advance(); // consume "["
                    
                    while (current_ < tokens_.size() && peek().lexeme != "]") {
                        if (peek().lexeme == "edad") {
                            advance(); // "edad"
                            advance(); // ":"
                            p.age = std::stoi(advance().lexeme);
                        } else if (peek().lexeme == "tipo_sangre") {
                            advance(); // "tipo_sangre"
                            advance(); // ":"
                            p.bloodType = advance().lexeme;
                        } else {
                            advance();
                        }
                    }
                    
                    if (peek().lexeme == "]") advance(); // consume "]"
                    if (peek().lexeme == ",") advance(); // consume ","
                    
                    patients_.push_back(p);
                } else {
                    advance();
                }
            }
            if (peek().lexeme == "}") advance();
        }
        
        // Buscar MEDICOS
        else if (token.lexeme == "MEDICOS") {
            advance(); // consume "MEDICOS"
            advance(); // consume "{"
            
            while (current_ < tokens_.size() && peek().lexeme != "}") {
                if (peek().lexeme == "medico") {
                    advance(); // consume "medico"
                    advance(); // consume ":"
                    
                    Doctor d;
                    d.name = advance().lexeme; // nombre
                    advance(); // consume "["
                    
                    while (current_ < tokens_.size() && peek().lexeme != "]") {
                        if (peek().lexeme == "especialidad") {
                            advance(); // "especialidad"
                            advance(); // ":"
                            d.specialty = advance().lexeme;
                        } else {
                            advance();
                        }
                    }
                    
                    if (peek().lexeme == "]") advance(); // consume "]"
                    if (peek().lexeme == ",") advance(); // consume ","
                    
                    doctors_.push_back(d);
                } else {
                    advance();
                }
            }
            if (peek().lexeme == "}") advance();
        }
        
        // Buscar CITAS
        else if (token.lexeme == "CITAS") {
            advance(); // consume "CITAS"
            advance(); // consume "{"
            
            while (current_ < tokens_.size() && peek().lexeme != "}") {
                if (peek().lexeme == "cita") {
                    advance(); // consume "cita"
                    advance(); // consume ":"
                    
                    Appointment a;
                    a.patient = advance().lexeme; // nombre paciente
                    advance(); // consume "con"
                    a.doctor = advance().lexeme; // nombre doctor
                    advance(); // consume "["
                    
                    while (current_ < tokens_.size() && peek().lexeme != "]") {
                        if (peek().lexeme == "fecha") {
                            advance(); // "fecha"
                            advance(); // ":"
                            a.date = advance().lexeme;
                        } else if (peek().lexeme == "hora") {
                            advance(); // "hora"
                            advance(); // ":"
                            a.time = advance().lexeme;
                        } else {
                            advance();
                        }
                    }
                    
                    if (peek().lexeme == "]") advance(); // consume "]"
                    if (peek().lexeme == ",") advance(); // consume ","
                    
                    appointments_.push_back(a);
                } else {
                    advance();
                }
            }
            if (peek().lexeme == "}") advance();
        }
        
        else {
            advance();
        }
    }
}

const std::vector<Patient>& MedLangParser::getPatients() const {
    return patients_;
}

const std::vector<Doctor>& MedLangParser::getDoctors() const {
    return doctors_;
}

const std::vector<Appointment>& MedLangParser::getAppointments() const {
    return appointments_;
}