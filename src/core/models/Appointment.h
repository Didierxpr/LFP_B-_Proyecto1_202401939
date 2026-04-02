#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

struct Appointment {
    std::string patient;
    std::string doctor;
    std::string date;
    std::string time;
};

#endif