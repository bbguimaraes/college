#ifndef MASS_H
#define MASS_H

#include <set>

#include "sms/vector.h"

class Spring;

class Mass {
    Vector m_position;
    Vector m_speed;
    double m_mass;
    bool m_fixed;
    Vector m_force;
    std::set<Spring *> m_springs;

    public:
        Mass() :
            m_position(Vector()),
            m_speed(0.0f),
            m_mass(1.0f),
            m_fixed(false),
            m_force(0.0f),
            m_springs(std::set<Spring *>()) {}
        Mass(
                Vector position,
                Vector speed,
                float mass,
                bool fixed,
                Vector force,
                const std::set<Spring *> springs) :
            m_position(position),
            m_speed(speed),
            m_mass(mass),
            m_fixed(fixed),
            m_force(force),
            m_springs(springs) {}
        void set_position(const Vector & p) {this->m_position = p;}
        void set_speed(const Vector & speed) {this->m_speed = speed;}
        void set_mass(double mass) {this->m_mass = mass;}
        void set_fixed(bool fixed) {this->m_fixed = fixed;}
        void set_force(const Vector & force) {this->m_force = force;}
        void set_springs(const std::set<Spring *> & s) {this->m_springs = s;}

        Vector position() const {return m_position; }
        Vector speed() const {return m_speed; }
        double mass() const {return m_mass; }
        bool fixed() const {return m_fixed; }
        Vector force() const {return m_force; }
        std::set<Spring *>  * springs() {return &m_springs;}
        const std::set<Spring *>  * springs() const {return &m_springs;}
};

#endif // MASS_H
