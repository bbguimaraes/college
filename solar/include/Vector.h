#ifndef VECTOR_H
#define VECTOR_H

struct Vector {
    float m_x, m_y, m_z;

    public:
        Vector(float x = 0, float y = 0, float z = 0)
            : m_x(x), m_y(y), m_z(z) {}
        float x() const {return this->m_x;}
        float y() const {return this->m_y;}
        float z() const {return this->m_z;}
        void set_x(float v) {this->m_x = v;}
        void set_y(float v) {this->m_y = v;}
        void set_z(float v) {this->m_z = v;}
};

Vector operator-(const Vector & rhs, const Vector & lhs);
Vector operator+(const Vector & rhs, const Vector & lhs);
Vector & operator+=(Vector & rhs, const Vector & lhs);

#endif // VECTOR_H
