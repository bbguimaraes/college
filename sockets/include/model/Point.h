#ifndef POINT_H
#define POINT_H

class Point {
    int m_x, m_y;

    public:
        Point() : m_x(0), m_y(0) {}
        Point(int x, int y) : m_x(x), m_y(y) {}

        int x() const {return m_x;}
        int y() const {return m_y;}

        int & rx() {return m_x;}
        int & ry() {return m_y;}
};

bool operator ==(const Point & p1, const Point & p2);
bool operator !=(const Point & p1, const Point & p2);
Point operator +(const Point & p1, const Point & p2);
Point operator -(const Point & p1, const Point & p2);

#endif // POINT_H
