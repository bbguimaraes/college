#ifndef VECTOR_H
#define VECTOR_H

class Vector {
    float m_x, m_y, m_z;

    public:
        Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f);
        float x() const;
        float y() const;
        float z() const;
        float magnitude() const;

        Vector normalized() const;
        Vector scaled(float n) const;
};

Vector operator+(const Vector & lhs, const Vector & rhs);
Vector operator-(const Vector & lhs, const Vector & rhs);
Vector operator*(float lhs, const Vector & rhs);
Vector operator*(const Vector & lhs, float rhs);
Vector operator/(const Vector & lhs, float rhs);
Vector operator/(float lhs, const Vector & rhs);

#endif // VECTOR_H
