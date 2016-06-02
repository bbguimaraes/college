#ifndef PLANET_H
#define PLANET_H

#include <ostream>
#include <string>

#include <GL/glu.h>

class OpenGLTexture;

class Planet {
    public:
        static const float AU_TO_KM;

        Planet(
            std::string name,
            float distance,
            float speed = 0.0f,
            float radius = 1.0f,
            OpenGLTexture * texture = nullptr,
            bool star = false);

        void set_name(std::string name) {this->m_name = name;}
        void set_distance(float distance) {this->m_distance = distance;}
        void set_speed(float speed) {this->m_speed = speed;}
        void set_radius(float radius) {this->m_radius = radius;}
        void set_texture(OpenGLTexture * texture) {this->m_texture = texture;}
        void set_star(bool star) {this->m_star = star;}

        std::string name() const {return this->m_name;}
        float distance() const {return this->m_distance;}
        float speed() const {return this->m_speed;}
        float angle() const {return this->m_angle;}
        float radius() const {return this->m_radius;}
        const OpenGLTexture * texture() const {return this->m_texture;}
        bool star() const {return this->m_star;}

        void update(float time);
        void draw(GLUquadric * quadric, unsigned int slices) const;

    private:
        std::string m_name;
        float m_distance;
        float m_speed;
        float m_angle;
        float m_radius;
        OpenGLTexture * m_texture;
        bool m_star;
};

std::ostream & operator<<(std::ostream & os, const Planet & p);

#endif // PLANET_H
