#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <GL/gl.h>

class Vector;

struct OpenGLTexture {
    GLuint m_id;

    public:
        OpenGLTexture(
            const unsigned char * data,
            unsigned int format,
            unsigned int width,
            unsigned int height);
        void bind() const;
};

#endif // OPENGL_TEXTURE_H
