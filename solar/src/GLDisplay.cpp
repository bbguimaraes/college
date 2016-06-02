#include <GLDisplay.h>

#include <cmath>

#include <QKeyEvent>
#include <QMouseEvent>

#include "Camera.h"
#include "OpenGLTexture.h"
#include "Tile.h"

#include <SolarSystem.h>
#include <Planet.h>

static void draw_circle(float r, int seg = 30) {
    const GLfloat D = 6.28f / seg;
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < seg; ++i) {
        GLfloat a = D * i;
        glVertex2f(r * std::cos(a), r * std::sin(a));
    }
    glEnd();
}

const GLfloat BLACK[]     = {0.00f, 0.00f, 0.00f, 0.0f};
const GLfloat LIGHTGREY[] = {0.75f, 0.75f, 0.75f, 1.0f};
const GLfloat WHITE[]     = {1.00f, 1.00f, 1.00f, 1.0f};

GLDisplay::GLDisplay(QWidget * parent) :
        QGLWidget(parent),
        m_update_rate(1000.0f / 60.0f),
        m_zoom_speed(0.0f),
        m_background(nullptr),
        m_solar_system(nullptr),
        m_selected(nullptr),
        m_planet_list(0),
        m_paused(false),
        m_slow(false),
        m_ambient(true),
        m_diffuse(true),
        m_specular(true),
        m_shininess(50),
        m_local_viewer(true),
        m_line_smooth(false),
        m_perspective_corr(false),
        m_texture(true),
        m_last_frame(QTime::currentTime()),
        m_frame_count(0) {
    this->makeCurrent();
    this->m_planet_list = this->create_planet_list(64);
    this->m_planet_select_list = this->create_planet_list(16);
    QImage bg = QGLWidget::convertToGLFormat(QImage("img/stars.jpg"));
    this->m_background =
        new OpenGLTexture(bg.bits(), GL_RGBA, bg.width(), bg.height());
}

/*virtual*/
void GLDisplay::keyPressEvent(QKeyEvent * event) {
    QGLWidget::keyPressEvent(event);
    this->m_keys_pressed.insert(event->key());
}

/*virtual*/
void GLDisplay::keyReleaseEvent(QKeyEvent * event) {
    QGLWidget::keyReleaseEvent(event);
    this->m_keys_pressed.erase(event->key());
}

bool GLDisplay::key_pressed(int key) const {
    return this->m_keys_pressed.find(key)
        != std::end(this->m_keys_pressed);
}

void GLDisplay::check_keys() {
    const bool CTRL = key_pressed(Qt::Key_Control);
    const float SCALE = 0.1f / (CTRL ? 10 : 1);
    if(key_pressed(Qt::Key_Up))
        m_camera.change_distance(-SCALE);
    if(key_pressed(Qt::Key_Down))
        m_camera.change_distance(SCALE);
    if(key_pressed(Qt::Key_Right))
        m_camera.rotate_y(-SCALE * 10.0f);
    if(key_pressed(Qt::Key_Left))
        m_camera.rotate_y(SCALE * 10.0f);
    if(key_pressed(Qt::Key_PageDown))
        m_camera.rotate_x(-SCALE * 10.0f);
    if(key_pressed(Qt::Key_PageUp))
        m_camera.rotate_x(SCALE * 10.0f);
    if(key_pressed('A'))
        m_camera.move_x(SCALE);
    if(key_pressed('D'))
        m_camera.move_x(-SCALE);
    if(key_pressed('S'))
        m_camera.move_z(-SCALE);
    if(key_pressed('W'))
        m_camera.move_z(SCALE);
}

/*virtual*/
void GLDisplay::mousePressEvent(QMouseEvent * event) {
    QGLWidget::mousePressEvent(event);
    this->m_last_position = Vector(event->x(), event->y());
    // required for the key presses to be recorded
    this->setFocus();
}

/*virtual*/
void GLDisplay::mouseReleaseEvent(QMouseEvent * event) {
    QGLWidget::mouseReleaseEvent(event);
    if(this->m_selected && event->button() == Qt::LeftButton)
        emit planet_selected(this->m_selected);
}

/*virtual*/
void GLDisplay::mouseMoveEvent(QMouseEvent * event) {
    Vector pos(event->x(), event->y());
    this->pick(pos);
    if(event->buttons() & Qt::RightButton) {
        Vector offset = pos - this->m_last_position;
        this->m_camera.rotate(offset.y(), offset.x(), 0.0f);
    }
    this->m_last_position = pos;
}

/*virtual*/
void GLDisplay::wheelEvent(QWheelEvent * event) {
    this->m_zoom_speed =
        -event->angleDelta().y() * m_camera.distance() / 480.0f;
}

GLuint GLDisplay::create_planet_list(int slices) {
    GLuint ret = glGenLists(1);
    glNewList(ret, GL_COMPILE);
    GLUquadric * quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    Planet("", 0.0f, 0.0f, 1.0f).draw(quadric, slices);
    glEndList();
    return ret;
}

void GLDisplay::toggle_diffuse() {
    this->m_diffuse = !this->m_diffuse;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, this->m_diffuse ? WHITE : BLACK);
}

void GLDisplay::toggle_ambient() {
    this->m_ambient = !this->m_ambient;
    static GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, this->m_ambient ? ambient : BLACK);
}

void GLDisplay::toggle_specular() {
    this->m_specular = !this->m_specular;
    glLightfv(GL_LIGHT0, GL_SPECULAR, this->m_specular ? WHITE : BLACK);
}

void GLDisplay::set_shininess(int shininess) {
    if(this->m_shininess == shininess)
        return;
    this->m_shininess = shininess;
    glMateriali(GL_FRONT, GL_SHININESS, this->m_shininess);
}

void GLDisplay::toggle_line_smooth() {
    this->m_line_smooth = !this->m_line_smooth;
    if(this->m_line_smooth) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
}

void GLDisplay::toggle_perspective_corr() {
    this->m_perspective_corr = !this->m_perspective_corr;
    glHint(
        GL_PERSPECTIVE_CORRECTION_HINT,
        this->m_perspective_corr ? GL_NICEST : GL_FASTEST);
}

/*virtual*/
void GLDisplay::update() {
    QGLWidget::update();
    this->check_keys();
    this->m_camera.change_distance(this->m_zoom_speed);
    this->m_zoom_speed /= 1.125f;
    if(this->m_paused)
        return;
    if(this->m_solar_system)
        for(Planet * planet : *this->m_solar_system->planets())
            planet->update(this->m_update_rate * (this->m_slow ? 0.1f : 1.0f));
    if(this->m_last_frame.elapsed() < 1000)
        ++this->m_frame_count;
    else {
        window()->setWindowTitle(
            ("solar - FPS: " + std::to_string(this->m_frame_count)).c_str());
        this->m_frame_count = 0;
        this->m_last_frame = QTime::currentTime();
    }
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        emit opengl_error(error);
}

/*virtual*/
void GLDisplay::initializeGL() {
    glPolygonMode(GL_BACK, GL_LINE);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  BLACK);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat materialSpecular[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR,  materialSpecular);
    glMateriali(GL_FRONT, GL_SHININESS, m_shininess);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

/*virtual*/
void GLDisplay::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<float>(w) / h, 0.01, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void GLDisplay::pick(const Vector & pos) {
    this->makeCurrent();
    glClear(GL_DEPTH_BUFFER_BIT);
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    const unsigned int SIZE = 512;
    GLuint selectBuf[SIZE];
    glSelectBuffer(SIZE, selectBuf);
    glRenderMode(GL_SELECT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(
        static_cast<GLdouble>(pos.x()),
        static_cast<GLdouble>(viewport[3] - pos.y()),
        1.0, 1.0,
        viewport);
    gluPerspective(
        60.0, static_cast<float>(viewport[2]) / viewport[3], 0.01, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glInitNames();
    glPushName(0);
    this->set_camera();
    this->draw_system(GL_SELECT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    GLint hits = glRenderMode(GL_RENDER);
    Planet * new_selected = nullptr;
    if(hits != 0)
        new_selected = this->m_solar_system->planets()->at(selectBuf[3]);
    if(this->m_selected != new_selected) {
        this->m_selected = new_selected;
        emit planet_highlighted(this->m_selected);
    }
}

void GLDisplay::set_camera() {
    glTranslatef(0.0f, 0.0f, -this->m_camera.distance());
    glRotated(this->m_camera.x_angle(), 1.0, 0.0, 0.0);
    glRotated(this->m_camera.y_angle(), 0.0, 1.0, 0.0);
    glRotated(this->m_camera.z_angle(), 0.0, 0.0, 1.0);
    glTranslatef(
        -this->m_camera.center().x(),
        -this->m_camera.center().y(),
        -this->m_camera.center().z());
}

/*virtual*/
void GLDisplay::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(this->m_background)
        this->draw_background();
    this->set_camera();
    if(this->m_solar_system) {
        for(Planet * p : *this->m_solar_system->planets()) {
            if(!p->star())
                continue;
            glPushMatrix();
            glRotatef(p->angle(), 0.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, p->distance());
            static const GLfloat origin[] = {0.0f, 0.0f, 0.0f, 1.0f};
            glLightfv(GL_LIGHT0, GL_POSITION, origin);
            glPopMatrix();
        }
        this->draw_system(GL_RENDER);
    }
    glFlush();
}

void GLDisplay::draw_background() const {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3fv(WHITE);
    glTranslatef(0.0f, 0.0f, -0.1f);
    Tile(this->m_background)
        .draw(
            Vector(-0.5f, -0.5f),
            Vector( 0.5f,  0.5f));
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void GLDisplay::draw_system(GLenum mode) const {
    int name = 0;
    for(auto planet : *this->m_solar_system->planets()) {
        if(mode == GL_SELECT)
            glLoadName(name++);
        else {
            if(planet->name() == "Moon")
                glMaterialfv(GL_FRONT, GL_EMISSION, LIGHTGREY);
            if(planet->texture())
                planet->texture()->bind();
        }
        glPushMatrix();
        if(mode != GL_SELECT) {
            if(this->m_texture)
                glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            draw_circle(planet->distance(), 90);
            glEnable(GL_LIGHTING);
            glPopMatrix();
            if(this->m_texture)
                glDisable(GL_TEXTURE_2D);
        }
        glRotatef(planet->angle(), 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, planet->distance());
        glScalef(planet->radius(), planet->radius(), planet->radius());
        if(mode != GL_SELECT) {
            if(planet->star())
                glMaterialfv(GL_FRONT, GL_EMISSION, WHITE);
            if(this->m_texture)
                glEnable(GL_TEXTURE_2D);
        }
        glCallList(
            mode != GL_SELECT
                ? this->m_planet_list
                : this->m_planet_select_list);
        if(mode != GL_SELECT) {
            if(this->m_texture)
                glDisable(GL_TEXTURE_2D);
            if(planet->star())
                glMaterialfv(GL_FRONT, GL_EMISSION, BLACK);
        }
        glPopMatrix();
        if(mode != GL_SELECT && planet->name() == "Moon")
            glMaterialfv(GL_FRONT, GL_EMISSION, BLACK);
    }
}
