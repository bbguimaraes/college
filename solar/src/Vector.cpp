#include "Vector.h"

Vector operator-(const Vector & rhs, const Vector & lhs) {
    return Vector(
        rhs.x() - lhs.x(),
        rhs.y() - lhs.y(),
        rhs.z() - lhs.z());
}

Vector operator+(Vector & rhs, const Vector & lhs) {
    return Vector(
        rhs.x() + lhs.x(),
        rhs.y() + lhs.y(),
        rhs.z() + lhs.z());
}

Vector & operator+=(Vector & rhs, const Vector & lhs) {
    rhs = rhs + lhs;
    return rhs;
}
