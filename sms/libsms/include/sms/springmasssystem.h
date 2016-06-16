#ifndef SPRING_MASS_SYSTEM_H
#define SPRING_MASS_SYSTEM_H

#include <vector>

#include "sms/vector.h"

class Mass;
class Spring;

class SpringMassSystem {
    static const float DAMPING;
    std::vector<Mass *> m_masses;
    std::vector<Spring *> m_springs;
    Vector m_gravity;

    public:
        SpringMassSystem() : m_gravity(Vector(0.0f, -9.8f, 0.0f)) {}

        void set_masses(const std::vector<Mass *> & m)
            {this->m_masses = m;}
        void set_springs(const std::vector<Spring *> & s)
            {this->m_springs = s;}
        void set_gravity(const Vector & g) {this->m_gravity = g;}

        const std::vector<Mass *> & masses() const {return this->m_masses;}
        const std::vector<Spring *> & springs() const {return this->m_springs;}
        Vector gravity() const {return this->m_gravity;}

        static void link(Mass * mass0, Mass * mass1, Spring * spring);
        void update(float interval);
};

#endif // SPRING_MASS_SYSTEM_H
