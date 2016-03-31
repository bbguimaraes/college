#include "gui/Texture.h"

#include <QImage>
#include <QGLWidget>

Texture::Texture(const std::string & filename) {
    QImage image = QGLWidget::convertToGLFormat(QImage(filename.c_str()));
    glGenTextures(1, &this->m_id);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGBA,
        GL_UNSIGNED_BYTE, image.bits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const {
    if(this->is_texture())
        glBindTexture(GL_TEXTURE_2D, m_id);
}

bool Texture::is_texture() const {
    return glIsTexture(m_id);
}
