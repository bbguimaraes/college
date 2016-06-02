#include "OpenGLTexture.h"

OpenGLTexture::OpenGLTexture(
        const unsigned char * data,
        unsigned int format,
        unsigned int width,
        unsigned int height) :
            m_id(0) {
    glGenTextures(1, &this->m_id);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
    glTexImage2D(
        GL_TEXTURE_2D,    // Target
        0,                // Level of detail for mipmaps (0 == base image)
        GL_RGBA,          // Format OpenGL will use to store texture
        width,            // Width
        height,           // Height
        0,                // Border
        format,           // Array format
        GL_UNSIGNED_BYTE, // Array type
        data              // Image data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void OpenGLTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}
