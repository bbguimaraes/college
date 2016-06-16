#ifndef SMSCREATOR_H
#define SMSCREATOR_H

#include <string>
#include <istream>

#include "sms/vector.h"

class SpringMassSystem;

class SMSCreator {
    public:
        static void read_from_file(
            SpringMassSystem * system, std::istream * input);
        static void create_string(
            SpringMassSystem * system,
            Vector begin, Vector end,
            unsigned int n_masses, float total_mass, float k);
        static void create_flag(
            SpringMassSystem * system,
            Vector begin, Vector end,
            unsigned int x_masses, unsigned int y_masses,
            float total_mass, float k);
        static void create_crossed_flag(
            SpringMassSystem * system,
            Vector begin, Vector end,
            unsigned int x_masses, unsigned int y_masses,
            float total_mass, float k);
};

#endif // SMSCREATOR_H
