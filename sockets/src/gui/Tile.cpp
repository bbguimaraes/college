#include "gui/Tile.h"

#include <GL/gl.h>

#include "gui/Color.h"
#include "gui/Texture.h"

/*virtual*/
void Tile::draw(const Color & color) const {
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glTranslatef(-0.5f, -0.5f, 0.0f);
    glColor4f(color.r(), color.g(), color.b(), color.a());
    if(this->m_texture != nullptr)
        this->m_texture->bind();
    else
        glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i  (0, 0);
    glTexCoord2i(1, 0);
    glVertex2i  (1, 0);
    glTexCoord2i(1, 1);
    glVertex2i  (1, 1);
    glTexCoord2i(0, 1);
    glVertex2i  (0, 1);
    glEnd();
    glPopMatrix();
    glPopAttrib();
}
