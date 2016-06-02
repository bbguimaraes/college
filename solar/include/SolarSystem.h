#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <vector>

class Planet;

class SolarSystem {
    std::vector<Planet *> m_planets;

    public:
        SolarSystem() {}
        SolarSystem(std::vector<Planet *> planets) : m_planets(planets) {}

        void set_planets(std::vector<Planet *> planets)
            {this->m_planets = planets;}
        std::vector<Planet *> * planets() { return &this->m_planets; }
        const std::vector<Planet *> * planets() const
            {return &this->m_planets;}
};

#endif // SOLAR_SYSTEM_H
