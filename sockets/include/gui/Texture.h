#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

#include <string>

class Texture {
    public:
        Texture(const std::string & filename);
        ~Texture();
        void bind() const;
        bool is_texture() const;

    private:
        Texture(const Texture &) = delete;
        void operator =(const Texture &) = delete;

        GLuint m_id;
};

#endif // TEXTURE_H
