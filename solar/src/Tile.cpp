#include "Tile.h"

#include <GL/gl.h>

#include "OpenGLTexture.h"
#include "Vector.h"

void Tile::draw(const Vector & bl, const Vector & tr) const {
    float w = tr.x() - bl.x();
    float h = tr.y() - bl.y();
    float x = bl.x() + w / 2.0f;
    float y = bl.y() + h / 2.0f;
    this->m_texture->bind();
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(w, h, 1.0f);
    glTranslatef(-0.5f, -0.5f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glTexCoord2i(1, 0); glVertex2i(1, 0);
    glTexCoord2i(1, 1); glVertex2i(1, 1);
    glTexCoord2i(0, 1); glVertex2i(0, 1);
    glEnd();
    glPopMatrix();
}
