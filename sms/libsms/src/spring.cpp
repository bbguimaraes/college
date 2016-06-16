#include "sms/spring.h"
#include "sms/mass.h"

void Spring::update_force() {
    this->m_distance = this->mass1()->position() - this->mass0()->position();
    float displacement = this->distance().magnitude() - this->eq_length();
    // Hooke's law: F = kX
    float force = this->k() * displacement;
    // TODO remove normalization?
    // can the force be derived from the original value of distance?
    this->m_force = this->distance().normalized().scaled(-force);
}
