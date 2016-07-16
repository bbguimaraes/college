#include "sms/spring.h"
#include "sms/mass.h"

void Spring::update_force() {
    this->m_distance = this->mass1()->position() - this->mass0()->position();
    const float DISTANCE_MAG = this->distance().magnitude();
    // Hooke's law: F = kX
    float force = this->k() * (DISTANCE_MAG - this->eq_length());
    this->m_force = this->distance().scaled(-force / DISTANCE_MAG);
}
