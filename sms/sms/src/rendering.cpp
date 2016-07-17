#include "rendering.h"

#include <cmath>

#include <GL/glu.h>

#include "sms/springmasssystem.h"
#include "camera.h"

/*static*/
void Rendering::set_camera(const Camera & c) {
    glTranslatef(0.0f, 0.0f, -c.distance());
    auto v = c.center();
    glTranslatef(v.x(), v.y(), v.z());
    v = c.rotation();
    glRotatef(-v.x(), 1.0, 0.0, 0.0);
    glRotatef(-v.y(), 0.0, 1.0, 0.0);
    glRotatef(-v.z(), 0.0, 0.0, 1.0);
}

/*static*/
void Rendering::draw_grid(unsigned int size) {
    const int I_SIZE = static_cast<int>(size);
    glBegin(GL_LINES);
    for(int i = -I_SIZE + 1; i < I_SIZE; i++) {
        if(!i)
            glColor3f(0.0f, 0.5f, 0.0f);
        else
            glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(i, -I_SIZE);
        glVertex2f(i,  I_SIZE);
        glVertex2f(-I_SIZE, i);
        glVertex2f( I_SIZE, i);
    }
    glEnd();
}

/*static*/
void Rendering::draw_circle(unsigned int slices) {
    const float SLICES_F = static_cast<float>(slices);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for(unsigned int i = 0; i <= slices; ++i)
        glVertex2f(
            std::cos(i / SLICES_F * 6.28318f),
            std::sin(i / SLICES_F * 6.28318f));
    glEnd();
}

/*static*/
void Rendering::draw_axis(GLUquadric * q, unsigned int slices) {
    glPushMatrix();
    glScalef(0.025f, 0.025f, 1.0f);
    Rendering::draw_circle(slices);
    gluCylinder(q, 1.0f, 1.0f, 0.9f, slices, slices);
    glTranslatef(0.0f, 0.0f, 0.9f);
    glScalef(2.0f, 2.0f, 1.0f);
    Rendering::draw_circle(slices);
    gluCylinder(q, 1.0f, 0.0f, 0.1f, slices, slices);
    glPopMatrix();
}

/*static*/
void Rendering::draw_axes(GLUquadric * q, unsigned int slices) {
    glPushAttrib(GL_CURRENT_BIT);
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.5f);
    Rendering::draw_axis(q, slices);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.0f, 0.0f);
    Rendering::draw_axis(q, slices);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.5f, 0.0f);
    Rendering::draw_axis(q, slices);
    glPopMatrix();
    glPopAttrib();
}

/*static*/
void Rendering::draw_springs_non_textured(const SpringMassSystem * system) {
    glBegin(GL_LINES);
    for(auto & spring : *system->springs()) {
        auto p0 = spring.mass0()->position();
        auto p1 = spring.mass1()->position();
        glVertex3f(p0.x(), p0.y(), p0.z());
        glVertex3f(p1.x(), p1.y(), p1.z());
    }
    glEnd();
}

/*static*/
void Rendering::draw_springs_textured(
        SpringMassSystem * system,
        unsigned int texture_width, unsigned int texture_height) {
    auto masses = system->masses();
    const float WIDTH_F = static_cast<float>(texture_width - 1);
    const float HEIGHT_F = static_cast<float>(texture_height - 1);
    auto point =
        [WIDTH_F, HEIGHT_F, texture_width, texture_height, masses]
        (int x, int y) {
            glTexCoord2f(x / WIDTH_F, y / HEIGHT_F);;
            auto p = (*masses)[y * texture_width + x].position();
            glVertex3f(p.x(), p.y(), p.z());;
        };
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    for(unsigned int y = 0; y < masses->size() / texture_width - 1; ++y)
        for(unsigned int x = 0; x < texture_width - 1; ++x) {
            point(x    , y    );
            point(x    , y + 1);
            point(x + 1, y + 1);
            point(x    , y    );
            point(x + 1, y + 1);
            point(x + 1, y    );
        }
    glEnd();
    glPopAttrib();
}

/*static*/
void Rendering::draw_masses(
        const SpringMassSystem * system, GLenum mode, GLUquadric * q,
        const Mass * selected) {
    glPushAttrib(GL_CURRENT_BIT);
    unsigned int index = 0;
    for(auto & mass : *system->masses()) {
        if(mode == GL_SELECT)
            glLoadName(index++);
        glPushMatrix();
        if(&mass == selected)
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(
            mass.position().x(),
            mass.position().y(),
            mass.position().z());
        gluSphere(q, 0.1, 4, 4);
        glPopMatrix();
    }
    if(mode == GL_SELECT)
        glPopName();
    glPopAttrib();
}

/*static*/
void Rendering::draw_number(unsigned int n) {
    static const std::vector<std::pair<int, int>> COORDS[10] = {
        {{ 0,  0}, {10,  0}, {10, 20},  {0, 20},  {0,  0}},
        {{ 5,  0}, { 5, 20}},
        {{10,  0}, { 0,  0}, { 0, 10}, {10, 10}, {10, 20}, { 0, 20}},
        {{ 0,  0}, {10,  0}, {10, 10}, { 0, 10}, {10, 10}, {10, 20}, { 0, 20}},
        {{10,  0}, {10, 20}, {10, 10}, { 0, 10}, { 0, 20}},
        {{ 0,  0}, {10,  0}, {10, 10}, { 0, 10}, { 0, 20}, {10, 20}},
        {{ 0, 20}, { 0,  0}, {10,  0}, {10, 10}, { 0, 10}},
        {{10,  0}, {10, 20}, { 0, 20}},
        {{ 0,  0}, {10,  0}, {10, 20}, { 0, 20}, { 0,  0}, { 0, 10}, {10, 10}},
        {{10,  0}, {10, 20}, { 0, 20}, { 0, 10}, {10, 10}},
    };
    if(n > 9) {
        Rendering::draw_number(n / 10);
        glTranslatef(30.0f, 0.0f, 0.0f);
    }
    glBegin(GL_LINE_STRIP);
    for(auto x : COORDS[n % 10])
        glVertex2f(x.first, x.second);
    glEnd();
}

/*static*/
void Rendering::draw_hud(unsigned int fps) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(5.0f, 5.0f, 0.0f);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    Rendering::draw_number(fps);
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
