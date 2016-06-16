#include "display.h"

#include <cassert>
#include <cmath>

#include <GL/glu.h>

#include <QWheelEvent>

#include "sms/springmasssystem.h"
#include "sms/mass.h"
#include "sms/spring.h"

Display::Display(QWidget * parent) :
        QGLWidget(parent),
        m_selected(nullptr),
        m_update_rate(1.0f),
        m_texture(0),
        m_quadric(nullptr),
        m_last_frame(QTime::currentTime()),
        m_ctrl_key_down(false),
        m_paused(false),
        m_draw_grid(false),
        m_draw_axes(false),
        m_draw_masses(true),
        m_textured(false),
        m_grid_size(100) {
    this->setFocusPolicy(Qt::StrongFocus);
}

Display::~Display() {
    gluDeleteQuadric(this->m_quadric);
}

/*virtual*/
void Display::initializeGL() {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0);
}

/*virtual*/
void Display::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0, static_cast<double>(w) / h, 0.1, 1000.0);
}

/*virtual*/
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->set_camera();
    if(this->m_draw_grid)
        this->draw_grid();
    if(this->m_draw_axes)
        this->draw_axes();
    for(auto x : this->m_systems)
        this->draw_system(x);
    this->update_fps();
    this->draw_hud();
}

void Display::set_camera() {
    glTranslatef(0.0f, 0.0f, -this->m_camera.distance());
    auto p = this->m_camera.center();
    glTranslatef(p.x(), p.y(), p.z());
    auto r = this->m_camera.rotation();
    glRotatef(-r.x(), 1.0, 0.0, 0.0);
    glRotatef(-r.y(), 0.0, 1.0, 0.0);
    glRotatef(-r.z(), 0.0, 0.0, 1.0);
}

void Display::select(Vector click) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const GLsizei BUFSIZE = 512;
    GLuint select_buffer[BUFSIZE];
    glSelectBuffer(BUFSIZE, select_buffer);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(
        static_cast<GLdouble>(click.x()),
        static_cast<GLdouble>(viewport[3]) - click.y(),
        5.0, 5.0,
        viewport);
    gluPerspective(
        60.0,
        static_cast<double>(viewport[2]) / viewport[3],
        0.1,
        1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->set_camera();
    Mass * selected = nullptr;
    for(auto system : this->m_systems) {
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);
        this->draw_masses(system, GL_SELECT);
        if(glRenderMode(GL_RENDER) != 0) {
            selected = (*system->masses())[select_buffer[3]];
            break;
        }
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    this->m_selected = selected;
}

/*slot*/
void Display::update_systems() {
    if(this->paused())
        return;
    for(auto x : this->m_systems)
        x->update(this->m_update_rate);
}

void Display::draw_grid() {
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT);
    glLineWidth(1.0f);
    GLfloat limit = static_cast<GLfloat>(this->m_grid_size);
    glBegin(GL_LINES);
    for(int i = 1; i < this->m_grid_size * 2; i++) {
        if(i == this->m_grid_size)
            glColor3f(0.0f, 0.5f, 0.0f);
        else
            glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f( limit - i, -limit    );
        glVertex2f( limit - i,  limit    );
        glVertex2f(-limit,      limit - i);
        glVertex2f( limit,      limit - i);
    }
    glEnd();
    glPopAttrib();
}

void Display::draw_axes() {
    const float SIZE = 5.0f;
    glPushAttrib(GL_CURRENT_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.5f);
    this->draw_axis(SIZE);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.0f, 0.0f);
    this->draw_axis(SIZE);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.5f, 0.0f);
    this->draw_axis(SIZE);
    glPopMatrix();
    glPopAttrib();
}

void Display::draw_axis(float height) {
    const float WIDTH = 0.1f;
    const unsigned int SLICES = 32;
    this->draw_circle(WIDTH);
    gluCylinder(this->m_quadric, WIDTH, WIDTH, height - 1.0f, SLICES, SLICES);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, height - 1.0f);
    this->draw_circle(WIDTH * 2.0f);
    gluCylinder(this->m_quadric, WIDTH * 2, 0.0f, 1.0f, SLICES, SLICES);
    glPopMatrix();
}

void Display::draw_circle(float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for(unsigned int i = 0; i <= 32; ++i)
        glVertex2f(
            std::cos(i / 32.0f * 6.28318f) * radius,
            std::sin(i / 32.0f * 6.28318f) * radius);
    glEnd();
}

void Display::draw_system(SpringMassSystem * system) {
    if(!this->textured())
        this->draw_springs_non_textured(system);
    else
        this->draw_springs_textured(system);
    if(this->m_draw_masses)
        this->draw_masses(system, GL_RENDER);
}

void Display::draw_springs_non_textured(SpringMassSystem * system) {
    glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
    glLineWidth(5.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    for(auto spring : *system->springs()) {
        auto p0 = spring->mass0()->position();
        auto p1 = spring->mass1()->position();
        glVertex3f(p0.x(), p0.y(), p0.z());
        glVertex3f(p1.x(), p1.y(), p1.z());
    }
    glEnd();
    glPopAttrib();
}

void Display::draw_springs_textured(SpringMassSystem * system) {
    assert(this->m_texture);
    auto masses = system->masses();
    const unsigned int WIDTH = 10;
    const float WIDTH_F = static_cast<float>(WIDTH);
    auto point = [WIDTH, WIDTH_F, masses](int x, int y) {
        glTexCoord2f(x / WIDTH_F, y / WIDTH_F);;
        auto p = (*masses)[y * WIDTH + x]->position();
        glVertex3f(p.x(), p.y(), p.z());;
    };
    glEnable(GL_TEXTURE_2D);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, this->m_texture);
    glBegin(GL_TRIANGLES);
    for(unsigned int y = 0; y < masses->size() / WIDTH - 1; ++y)
        for(unsigned int x = 0; x < WIDTH - 1; ++x) {
            point(x    , y    );
            point(x    , y + 1);
            point(x + 1, y + 1);
            point(x    , y    );
            point(x + 1, y + 1);
            point(x + 1, y    );
        }
    glEnd();
    glPopAttrib();
    glDisable(GL_TEXTURE_2D);
}

void Display::draw_masses(SpringMassSystem * system, GLenum mode) {
    if(!this->m_quadric)
        this->m_quadric = gluNewQuadric();
    glPushAttrib(GL_CURRENT_BIT);
    unsigned int index = 0;
    for(auto mass : *system->masses()) {
        if(mode == GL_SELECT)
            glLoadName(index++);
        glPushMatrix();
        if(mass == this->m_selected)
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(
            mass->position().x(),
            mass->position().y(),
            mass->position().z());
        gluSphere(this->m_quadric, 0.1, 16, 16);
        glPopMatrix();
    }
    if(mode == GL_SELECT)
        glPopName();
    glPopAttrib();
}

void Display::draw_hud() {
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
    this->draw_number(this->m_fps);
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void Display::draw_number(unsigned int n) {
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
        this->draw_number(n / 10);
        glTranslatef(30.0f, 0.0f, 0.0f);
    }
    glBegin(GL_LINE_STRIP);
    for(auto x : COORDS[n % 10])
        glVertex2f(x.first, x.second);
    glEnd();
}

void Display::update_fps() {
    if(this->m_last_frame.elapsed() < 1000)
        ++this->m_frame_count;
    else {
        this->m_fps = this->m_frame_count;
        this->m_frame_count = 0;
        this->m_last_frame = QTime::currentTime();
    }
}

void Display::keyPressEvent(QKeyEvent * event) {
    switch(event->key()) {
        case 'A':
            this->m_draw_axes = !this->m_draw_axes;
            break;
        case 'G':
            this->m_draw_grid = !this->m_draw_grid;
            break;
        case 'M':
            this->m_draw_masses = !this->m_draw_masses;
            break;
        case 'P':
            this->m_paused = !this->m_paused;
            break;
        case 'T':
            this->m_textured = !this->m_textured;
            break;
        case Qt::Key_Control:
            this->m_ctrl_key_down = true;
            break;
    }
}

void Display::keyReleaseEvent(QKeyEvent * event) {
    if(event->key() == Qt::Key_Control)
        this->m_ctrl_key_down = false;
}

void Display::mousePressEvent(QMouseEvent * event) {
    this->m_click_position = Vector(event->x(), event->y());
    if(event->button() == Qt::LeftButton)
        this->select(this->m_click_position);
}

void Display::mouseReleaseEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton && this->m_selected)
        this->m_selected->set_force(Vector());
}

void Display::mouseMoveEvent(QMouseEvent * event) {
    auto position = Vector(event->x(), event->y());
    if(!this->m_ctrl_key_down) {
        auto move = position - this->m_last_position;
        if(event->buttons() & Qt::LeftButton) {
            move = move * this->m_camera.distance() / 1000.0f;
            move = Vector(move.x(), -move.y(), move.z());
            this->m_camera.move(move);
        } else if(event->buttons() & Qt::RightButton) {
            move = move * -0.5;
            this->m_camera.rotate(Vector(move.y(), move.x()));
        }
    } else {
        if(!(event->buttons() & Qt::LeftButton))
            this->select(position);
        else if(this->m_selected) {
            auto move = (this->m_click_position - position)
                / this->m_camera.distance();
            this->m_selected->set_force(Vector(-move.x(), move.y()));
        }
    }
    this->m_last_position = position;
}

void Display::wheelEvent(QWheelEvent * event) {
    this->m_camera.change_distance(-0.01f * event->delta());
}
