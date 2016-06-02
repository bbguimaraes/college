#ifndef TILE_H
#define TILE_H

class OpenGLTexture;
class Vector;

class Tile {
    const OpenGLTexture * m_texture;

    public:
        Tile(const OpenGLTexture * t) : m_texture(t) {}
        void draw(const Vector & bl, const Vector & tr) const;
};

#endif // TILE_H
