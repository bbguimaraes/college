#ifndef CAMERA_H
#define CAMERA_H

#include "sms/vector.h"

class Camera {
    Vector m_center;
    Vector m_rotation;
    float m_distance;

    public:
        Camera() : m_distance(0.0f) {}

        Vector center() const {return this->m_center;}
        Vector rotation() const {return this->m_rotation;}
        float distance() const {return this->m_distance;}

        void set_distance(float d) {this->m_distance = d;}
        void change_distance(float d) {this->m_distance += d;}
        void move(Vector m) {this->m_center = this->m_center + m;}
        void rotate(Vector r) {this->m_rotation = this->m_rotation + r;}
};

#endif // CAMERA_H
