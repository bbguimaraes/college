#include "simulation.h"

#include "sms/springmasssystem.h"

void Simulation::update() {
    if(this->paused())
        return;
    for(auto x : this->m_systems)
        x->update(this->update_rate());
}
