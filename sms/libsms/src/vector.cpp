#include "sms/vector.h"

#include <cmath>

Vector::Vector(float x, float y, float z)
    : m_x(x), m_y(y), m_z(z) {}

float Vector::x() const {
    return this->m_x;
}

float Vector::y() const {
    return this->m_y;
}

float Vector::z() const {
    return this->m_z;
}

float Vector::magnitude() const {
    return std::sqrt(
        this->x() * this->x()
        + this->y() * this->y()
        + this->z() * this->z());
}

Vector Vector::normalized() const {
    auto m = this->magnitude();
    return Vector(
        this->x() / m,
        this->y() / m,
        this->z() / m);
}

Vector Vector::scaled(float n) const {
    return Vector(
        this->x() * n,
        this->y() * n,
        this->z() * n);
}

Vector operator+(const Vector & lhs, const Vector & rhs) {
    return Vector(
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z());
}

Vector operator-(const Vector & lhs, const Vector & rhs) {
    return Vector(
        lhs.x() - rhs.x(),
        lhs.y() - rhs.y(),
        lhs.z() - rhs.z());
}

Vector operator*(const Vector & lhs, float rhs) {
    return Vector(
        lhs.x() * rhs,
        lhs.y() * rhs,
        lhs.z() * rhs);
}

Vector operator*(float lhs, const Vector & rhs) {
    return Vector(
        lhs * rhs.x(),
        lhs * rhs.y(),
        lhs * rhs.z());
}

Vector operator/(const Vector & lhs, float rhs) {
    return Vector(
        lhs.x() / rhs,
        lhs.y() / rhs,
        lhs.z() / rhs);
}

Vector operator/(float lhs, const Vector & rhs) {
    return Vector(
        lhs / rhs.x(),
        lhs / rhs.y(),
        lhs / rhs.z());
}
