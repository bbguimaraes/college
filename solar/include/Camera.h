#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

class Camera {
    Vector m_center;
    float m_distance, m_x, m_y, m_z;

    public:
        Vector center() const {return this->m_center;}
        float distance() const {return this->m_distance;}
        float x_angle() const {return this->m_x;}
        float y_angle() const {return this->m_y;}
        float z_angle() const {return this->m_z;}

        void set_distance(float d) {this->m_distance = d;}
        void change_distance(float d) {this->m_distance += d;}
        void set_x_angle(float a) {this->m_x = a;}
        void set_y_angle(float a) {this->m_y = a;}
        void set_z_angle(float a) {this->m_z = a;}

        void move_x(float d) {this->m_center += Vector(d, 0, 0);}
        void move_y(float d) {this->m_center += Vector(0, d, 0);}
        void move_z(float d) {this->m_center += Vector(0, 0, d);}

        void rotate(float x, float y, float z) {
            this->rotate_x(x);
            this->rotate_y(y);
            this->rotate_z(z);
        }
        void rotate_x(float d) {this->m_x += d;}
        void rotate_y(float d) {this->m_y += d;}
        void rotate_z(float d) {this->m_z += d;}
};

#endif // CAMERA_H
