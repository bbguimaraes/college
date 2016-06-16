#include "sms/springmasssystem.h"

#include "sms/vector.h"
#include "sms/mass.h"
#include "sms/spring.h"

/*static*/
const float SpringMassSystem::DAMPING = 0.95f;

/*static*/
void SpringMassSystem::link(Mass * mass0, Mass * mass1, Spring * spring) {
    mass0->springs()->insert(spring);
    mass1->springs()->insert(spring);
    spring->set_mass0(mass0);
    spring->set_mass1(mass1);
}

void SpringMassSystem::update(float interval) {
    for(auto mass : *this->masses()) {
        if(mass->fixed())
            continue;
        Vector total_force = mass->force() + (this->m_gravity * mass->mass());
        for(auto spring : *mass->springs())
            total_force = total_force + (mass == spring->mass1()
                ? spring->force()
                : spring->force() * -1.0f);
        Vector speed = mass->speed() + total_force / mass->mass();
        speed = speed * this->DAMPING;
        mass->set_speed(speed);
        mass->set_position(mass->position() + speed * interval);
    }
    for(auto spring : *this->springs())
        spring->update_force();
}
