#include "model/Point.h"

bool operator ==(const Point & p1, const Point & p2) {
    return p1.x() == p2.x() && p1.y() == p2.y();
}

bool operator !=(const Point & p1, const Point & p2) {
    return !(p1 == p2);
}

Point operator +(const Point & p1, const Point & p2) {
    return {p1.x() + p2.x(), p1.y() + p2.y()};
}

Point operator -(const Point & p1, const Point & p2) {
    return {p1.x() - p2.x(), p1.y() - p2.y()};
}
