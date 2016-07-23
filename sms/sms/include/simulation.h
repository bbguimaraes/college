#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

class SpringMassSystem;

class Simulation {
    std::vector<SpringMassSystem *> m_systems;
    double m_update_rate;
    bool m_paused;
    bool m_visible;
    bool m_draw_masses;
    bool m_textured;
    bool m_texturable;
    unsigned int m_texture_width;
    unsigned int m_texture_height;
    bool m_wind;

    public:
        Simulation() :
            m_paused(false),
            m_visible(true),
            m_draw_masses(true),
            m_textured(false),
            m_texturable(false),
            m_texture_width(0),
            m_texture_height(0),
            m_wind(false) {}

        const std::vector<SpringMassSystem *> * systems() const
            {return &this->m_systems;}
        double update_rate() const {return this->m_update_rate;}
        bool paused() const {return this->m_paused;}
        bool visible() const {return this->m_visible;}
        bool draw_masses() const {return this->m_draw_masses;}
        bool textured() const {return this->m_textured;}
        bool texturable() const {return this->m_texturable;}
        unsigned int texture_width() const {return this->m_texture_width;}
        unsigned int texture_height() const {return this->m_texture_height;}
        bool wind() const {return this->m_wind;}

        void set_systems(const std::vector<SpringMassSystem *> & v)
            {this->m_systems = v;}
        void set_update_rate(double r) {this->m_update_rate = r;}
        void set_paused(bool p) {this->m_paused = p;}
        void set_visible(bool v) {this->m_visible = v;}
        void set_draw_masses(bool d) {this->m_draw_masses = d;}
        void set_textured(bool t) {this->m_textured = t;}
        void set_texturable(bool t) {this->m_texturable = t;}
        void set_texture_width(unsigned int w) {this->m_texture_width = w;}
        void set_texture_height(unsigned int h) {this->m_texture_height = h;}
        void set_wind(bool w) {this->m_wind = w;}

        void update();
};

#endif // SIMULATION_H
