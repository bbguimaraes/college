#ifndef SPRING_H
#define SPRING_H

#include "sms/vector.h"

class Mass;

class Spring {
    double m_k;
    double m_eq_length;
    Vector m_distance;
    Vector m_force;
    Mass * m_mass0;
    Mass * m_mass1;

    public:
        Spring() : m_mass0(nullptr), m_mass1(nullptr) {}

        void set_k(double k) {this->m_k = k;}
        void set_eq_length(double l) {this->m_eq_length = l;}
        void set_distance(const Vector & d) {this->m_distance = d;}
        void set_force(const Vector & f) {this->m_force = f;}
        void set_mass0(Mass * m) {this->m_mass0 = m;}
        void set_mass1(Mass * m) {this->m_mass1 = m;}

        double k() const {return this->m_k;}
        double eq_length() const {return this->m_eq_length;}
        Vector distance() const {return this->m_distance;}
        Vector force() const {return this->m_force;}
        Mass * mass0() {return this->m_mass0;}
        Mass * mass1() {return this->m_mass1;}

        void update_force();
};

#endif // SPRING_H
