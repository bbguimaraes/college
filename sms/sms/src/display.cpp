#include "display.h"

#include <GL/glu.h>

#include <QWheelEvent>

#include "sms/springmasssystem.h"
#include "rendering.h"

Display::Display(QWidget * parent) :
        QGLWidget(parent),
        m_selected(nullptr),
        m_texture(0),
        m_quadric(nullptr),
        m_last_select(QTime::currentTime()),
        m_last_frame(QTime::currentTime()),
        m_ctrl_key_down(false),
        m_draw_grid(false),
        m_draw_axes(false) {
    this->setFocusPolicy(Qt::StrongFocus);
}

Display::~Display() {
    if(this->m_quadric)
        gluDeleteQuadric(this->m_quadric);
}

/*virtual*/
void Display::initializeGL() {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0);
    this->m_quadric = gluNewQuadric();
}

/*virtual*/
void Display::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(60.0, static_cast<double>(w) / h, 0.1, 500.0);
}

/*virtual*/
void Display::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Rendering::set_camera(this->m_camera);
    if(this->draw_grid())
        Rendering::draw_grid(this->GRID_SIZE);
    if(this->draw_axes()) {
        glPushMatrix();
        glScalef(this->AXES_HEIGHT, this->AXES_HEIGHT, this->AXES_HEIGHT);
        Rendering::draw_axes(this->m_quadric, this->AXES_SLICES);
        glPopMatrix();
    }
    for(auto x : this->m_simulations)
        this->draw_simulation(x);
    this->update_fps();
    Rendering::draw_hud(this->m_fps);
}

void Display::select(Vector click, bool rate_limit) {
    if(rate_limit && this->m_last_select.elapsed() < this->SELECT_RATE_LIMIT)
        return;
    this->m_last_select = QTime::currentTime();
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
    Rendering::set_camera(this->m_camera);
    this->m_selected = nullptr;
    for(auto simulation : this->m_simulations)
        for(auto system : *simulation->systems()) {
            glRenderMode(GL_SELECT);
            glInitNames();
            glPushName(0);
            Rendering::draw_masses(
                system, GL_SELECT, this->m_quadric, this->m_selected);
            if(glRenderMode(GL_RENDER) != 0) {
                this->m_selected = &(*system->masses())[select_buffer[3]];
                break;
            }
        }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

/*slot*/
void Display::update_systems() {
    for(auto x : this->m_simulations)
        x->update();
}

void Display::draw_simulation(Simulation * s) {
    for(auto system : *s->systems()) {
        if(!s->textured()) {
            glPushAttrib(GL_CURRENT_BIT);
            glColor3f(0.0f, 1.0f, 0.0f);
            Rendering::draw_springs_non_textured(system);
            glPopAttrib();
        } else {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, this->m_texture);
            Rendering::draw_springs_textured(
                system, s->texture_width(), s->texture_height());
            glDisable(GL_TEXTURE_2D);
        }
        if(s->draw_masses())
            Rendering::draw_masses(
                system, GL_RENDER, this->m_quadric, this->m_selected);
    }
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
        case 'F':
            this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
            break;
        case 'G':
            this->m_draw_grid = !this->m_draw_grid;
            break;
        case 'M':
            for(auto x : this->m_simulations)
                x->set_draw_masses(!x->draw_masses());
            break;
        case 'P':
            for(auto x : this->m_simulations)
                x->set_paused(!x->paused());
            break;
        case 'T':
            for(auto x : this->m_simulations)
                if(x->texturable())
                    x->set_textured(!x->textured());
            break;
        case 'W':
            for(auto x : this->m_simulations)
                x->set_wind(!x->wind());
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
        this->select(this->m_click_position, false);
}

void Display::mouseReleaseEvent(QMouseEvent * event) {
    if(event->button() == Qt::LeftButton && this->m_selected)
        this->m_selected_force = Vector();
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
            this->select(position, true);
        else if(this->m_selected) {
            auto move = (this->m_click_position - position)
                / this->m_camera.distance();
            this->m_selected_force = Vector(-move.x(), move.y());
        }
    }
    this->m_last_position = position;
}

void Display::wheelEvent(QWheelEvent * event) {
    this->m_camera.change_distance(-0.01f * event->delta());
}
