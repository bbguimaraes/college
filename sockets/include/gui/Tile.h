#ifndef TILE_H
#define TILE_H

#include "model/Point.h"

class Color;
class Texture;

class Tile {
    public:
        Tile(const Texture * t) : m_texture(t) {}
        void set_texture(const Texture * t) {this->m_texture = t;}
        void draw(const Color & color) const;

        void set_position(const Point & position) {m_position = position;}
        Point position() const {return m_position;}

    private:
        const Texture * m_texture;
        Point m_position;
};

#endif // TILE_H
