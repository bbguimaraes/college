#include "smscreator.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

#include "sms/mass.h"
#include "sms/spring.h"
#include "sms/springmasssystem.h"

/**
 * Reads a SpringMassSystem from a file. Example of file:
 *
 * # begin
 * 5.0 4.0 0.0
 * # end
 * -5.0 4.0 0.0
 * # number of Masses
 * 51
 * # total mass
 * 1
 * # spring k constant
 * 1.0
 */
/*static*/
void SMSCreator::read_from_file(
        SpringMassSystem * system, std::istream * input) {
    auto next_line = [input]() -> std::string {
        std::string line;
        do {
            getline(*input, line);
        } while(line[0] == '#');
        return line;
    };
    auto read_point = [next_line]() -> Vector {
        auto ss = std::stringstream(next_line());
        float x, y, z;
        ss >> x >> y >> z;
        return Vector(x, y, z);
    };
    auto p0 = read_point();
    auto p1 = read_point();
    auto n_masses = std::stoi(next_line());
    auto total_mass = std::stof(next_line());
    auto k = std::stof(next_line());
    return SMSCreator::create_string(system, p0, p1, n_masses, total_mass, k);
}

/*static*/
void SMSCreator::create_string(
        SpringMassSystem * system,
        Vector begin, Vector end,
        unsigned int n_masses, float total_mass, float k) {
    const auto DIRECTION = end - begin;
    const auto SPRING_LENGTH =
        DIRECTION.magnitude() / static_cast<float>(n_masses - 1);
    const auto INCREMENT_VECTOR = DIRECTION.normalized() * SPRING_LENGTH;
    const auto EACH_MASS = total_mass / n_masses;
    auto masses = std::vector<Mass>(n_masses);
    auto springs = std::vector<Spring>(n_masses - 1);
    for(unsigned int i = 0; i < n_masses; ++i) {
        auto & mass = masses[i];
        mass.set_mass(EACH_MASS);
        mass.set_position(begin + (INCREMENT_VECTOR * static_cast<float>(i)));
    }
    for(auto & spring : springs) {
        spring.set_k(k);
        spring.set_eq_length(SPRING_LENGTH);
    }
    system->set_masses(masses);
    system->set_springs(springs);
    auto m0_it = std::begin(*system->masses());
    auto m1_it = ++std::begin(*system->masses());
    auto s_it = std::begin(*system->springs());
    while(m1_it != std::end(*system->masses()))
        SpringMassSystem::link(&(*m0_it++), &(*m1_it++), &(*s_it++));
}

/*static*/
void SMSCreator::create_flag(
        SpringMassSystem * system,
        Vector begin, Vector end,
        unsigned int x_masses, unsigned int y_masses,
        float total_mass, float k) {
    auto index = [x_masses](unsigned int x, unsigned int y)
        {return y * x_masses + x;};
    const Vector DIRECTION = end - begin;
    const Vector INCREMENT(
        DIRECTION.x() / (x_masses - 1),
        DIRECTION.y() / (y_masses - 1));
    const unsigned int N_MASSES = x_masses * y_masses;
    const float EACH_MASS = total_mass / N_MASSES;
    const unsigned int N_SPRINGS =
        x_masses * (y_masses - 1) + y_masses * (x_masses - 1);
    auto masses = std::vector<Mass>(N_MASSES);
    auto springs = std::vector<Spring>(N_SPRINGS);
    for(auto & spring : springs)
        spring.set_k(k);
    for(unsigned int y = 0; y < y_masses; ++y) {
        for(unsigned int x = 0; x < x_masses; ++x) {
            auto & mass = masses[index(x, y)];
            mass.set_mass(EACH_MASS);
            mass.set_position(
                begin + Vector(x * INCREMENT.x(), y * INCREMENT.y()));
        }
    }
    system->set_masses(masses);
    system->set_springs(springs);
    auto spring_it = std::begin(*system->springs());
    auto link = [system, index, &spring_it](\
            unsigned int x0, unsigned int y0,
            unsigned int x1, unsigned int y1,
            float eq_length) {
        spring_it->set_eq_length(std::abs(eq_length));
        SpringMassSystem::link(
            &(*system->masses())[index(x0, y0)],
            &(*system->masses())[index(x1, y1)],
            &(*spring_it));
        ++spring_it;
    };
    for(unsigned int y = 0; y < y_masses - 1; ++y) {
        for(unsigned int x = 0; x < x_masses - 1; ++x) {
            link(x, y, x + 1, y    , INCREMENT.x());
            link(x, y, x    , y + 1, INCREMENT.y());
        }
        link(x_masses - 1, y, x_masses - 1, y + 1, INCREMENT.y());
    }
    for(unsigned int x = 0; x < x_masses - 1; ++x)
        link(x, y_masses - 1, x + 1, y_masses - 1, INCREMENT.x());
}

void SMSCreator::create_crossed_flag(
        SpringMassSystem * system,
        Vector begin, Vector end,
        unsigned int x_masses, unsigned int y_masses,
        float total_mass, float k) {
    auto index = [x_masses](unsigned int x, unsigned int y)
        {return y * x_masses + x;};
    const Vector DIRECTION = end - begin;
    const Vector INCREMENT(
        DIRECTION.x() / (x_masses - 1),
        DIRECTION.y() / (y_masses - 1));
    const float INCREMENT_LENGTH = INCREMENT.magnitude();
    const unsigned int N_MASSES = x_masses * y_masses;
    const float EACH_MASS = total_mass / N_MASSES;
    const unsigned int N_SPRINGS =
        (x_masses * (y_masses - 1) + y_masses * (x_masses - 1))
        + (2 * (x_masses - 1) * (y_masses - 1));
    auto masses = std::vector<Mass>(N_MASSES);
    auto springs = std::vector<Spring>(N_SPRINGS);
    for(unsigned int y = 0; y < y_masses; ++y) {
        for(unsigned int x = 0; x < x_masses; ++x) {
            auto & mass = masses[index(x, y)];
            mass.set_mass(EACH_MASS);
            mass.set_position(
                begin + Vector(x * INCREMENT.x(), y * INCREMENT.y()));
        }
    }
    for(auto & spring : springs)
        spring.set_k(k);
    system->set_masses(masses);
    system->set_springs(springs);
    auto spring_it = std::begin(*system->springs());
    auto link = [system, index, &spring_it](\
            unsigned int x0, unsigned int y0,
            unsigned int x1, unsigned int y1,
            float eq_length) {
        spring_it->set_eq_length(std::abs(eq_length));
        SpringMassSystem::link(
            &(*system->masses())[index(x0, y0)],
            &(*system->masses())[index(x1, y1)],
            &(*spring_it));
        ++spring_it;
    };
    for(unsigned int y = 0; y < y_masses - 1; ++y) {
        for(unsigned int x = 0; x < x_masses - 1; ++x) {
            link(x    , y, x + 1, y    , INCREMENT.x());
            link(x    , y, x    , y + 1, INCREMENT.y());
            link(x    , y, x + 1, y + 1, INCREMENT_LENGTH);
            link(x + 1, y, x    , y + 1, INCREMENT_LENGTH);
        }
        link(x_masses - 1, y, x_masses - 1, y + 1, INCREMENT.y());
    }
    for(unsigned int x = 0; x < x_masses - 1; ++x)
        link(x, y_masses - 1, x + 1, y_masses - 1, INCREMENT.x());
}
