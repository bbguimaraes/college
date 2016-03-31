#include "gui/GameDisplay.h"

#include <GL/glu.h>

#include <QMouseEvent>
#include <QMessageBox>

#include "gui/Color.h"
#include "gui/GameController.h"
#include "gui/Texture.h"
#include "gui/Tile.h"
#include "model/Character.h"
#include "model/Game.h"
#include "model/Player.h"

const float GameDisplay::GRID_SIZE = 4.0f;
const float GameDisplay::WIDTH = 28.0f;
const float GameDisplay::HEIGHT = 36.0f;

GameDisplay::GameDisplay(QWidget * parent) :
        QGLWidget(parent), m_mode(GL_RENDER), m_angles{-30.0f, 0.0f, 0.0f} {
    this->setMouseTracking(true);
    this->makeCurrent();
    this->m_textures = this->load_textures();
    this->m_tiles = this->create_tiles();
    for(auto x : this->m_textures)
        this->m_character_tiles.push_back(new Tile(x));
}

GameDisplay::~GameDisplay() {
    for(auto x : this->m_character_tiles)
        delete x;
    for(auto x : this->m_textures)
        delete x;
}

std::vector<Texture *> GameDisplay::load_textures() {
    auto ret = std::vector<::Texture *>(12, nullptr);
    ret[Texture::RAT]      = new ::Texture("img/rat.png");
    ret[Texture::CAT]      = new ::Texture("img/cat.png");
    ret[Texture::DOG]      = new ::Texture("img/dog.png");
    ret[Texture::WOLF]     = new ::Texture("img/wolf.png");
    ret[Texture::PANTHER]  = new ::Texture("img/panther.png");
    ret[Texture::TIGER]    = new ::Texture("img/tiger.png");
    ret[Texture::LION]     = new ::Texture("img/lion.png");
    ret[Texture::ELEPHANT] = new ::Texture("img/elephant.png");
    ret[Texture::GRASS]    = new ::Texture("img/grass.png");
    ret[Texture::WATER]    = new ::Texture("img/water.png");
    ret[Texture::TRAP]     = new ::Texture("img/trap.png");
    ret[Texture::DEN]      = new ::Texture("img/den.png");
    return ret;
}

std::vector<Tile *> GameDisplay::create_tiles() {
    const int T = Texture::TRAP;
    const int W = Texture::WATER;
    const int D = Texture::DEN;
    const int G = Texture::GRASS;
    const int INIT[9][7] {
        {G, G, T, D, T, G, G},
        {G, G, G, T, G, G, G},
        {G, G, G, G, G, G, G},
        {G, W, W, G, W, W, G},
        {G, W, W, G, W, W, G},
        {G, W, W, G, W, W, G},
        {G, G, G, G, G, G, G},
        {G, G, G, T, G, G, G},
        {G, G, T, D, T, G, G},
    };
    auto ret = std::vector<Tile *>(63, nullptr);
    for(int y = 0; y < 9; ++y)
        for(int x = 0; x < 7; ++x) {
            auto p = Point{x, y};
            auto t = new Tile(this->m_textures[INIT[y][x]]);
            ret[y * 7 + x] = t;
            t->set_position(p);
        }
    return ret;
}

void GameDisplay::initializeGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameDisplay::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(
        45.0,
        static_cast<GLdouble>(w) / static_cast<GLdouble>(h),
        0.1, 100.0);
}

void GameDisplay::paintGL() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(0.0f, 3.2f, -40.0f);
    glRotatef(m_angles[0], 1.0f, 0.0f, 0.0f);
    glRotatef(m_angles[1], 0.0f, 1.0f, 0.0f);
    glRotatef(m_angles[2], 0.0f, 0.0f, 1.0f);
    this->draw_objects();
    if(this->m_mode == GL_RENDER)
        this->draw_grid();
}

void GameDisplay::draw_grid() {
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glTranslatef(-WIDTH / 2.0f, -HEIGHT / 2.0f, 0.1f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for(float i = 0.0f; i < HEIGHT + 1; i += GRID_SIZE) {
        glVertex3f( 0.0f, i, 0.0f);
        glVertex3f(WIDTH, i, 0.0f);
    }
    for(float i = 0.0f; i < WIDTH + 1; i += GRID_SIZE) {
        glVertex3f(i,  0.0f, 0.0f);
        glVertex3f(i, HEIGHT, 0.0f);
    }
    glEnd();
    glPopMatrix();
    glPopAttrib();
}

void GameDisplay::draw_objects() {
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT);
    glTranslatef(-WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f);
    for(int y = 0; y < 9; ++y) {
        for(int x = 0; x < 7; ++x) {
            if(this->m_mode == GL_SELECT)
                glPushName(y * 7 + x);
            const Point p(x, y);
            this->draw_tile(
                this->m_tiles[y * 7 + x],
                Color(1.0f, 1.0f, 1.0f),
                p);
            if(this->m_mode != GL_SELECT)
                this->draw_character(
                    this->m_controller->character_at(p),
                    p);
            if(this->m_mode == GL_SELECT)
                glPopName();
        }
    }
    glPopMatrix();
    glPopAttrib();
}

void GameDisplay::draw_tile(
        const Tile * t, const Color & c, const Point & p) {
    glPushMatrix();
    glScalef(GRID_SIZE, GRID_SIZE, GRID_SIZE);
    glTranslatef(static_cast<float>(p.x()), static_cast<float>(p.y()), 0.0f);
    glTranslatef(0.5f, 0.5f, 0.0f);
    if(p == this->m_controller->selected())
        t->draw(Color(1.0f, 0.0f, 0.0f));
    else if(this->should_highlight(p))
        t->draw(Color(0.0f, 1.0f, 0.0f));
    else
        t->draw(c);
    glPopMatrix();
}

void GameDisplay::draw_character(const Character * c, const Point & p) {
    if(!c)
        return;
    auto color = c->owner()->id() == 1
        ? Color(1.0f, 0.75f, 0.75f)
        : Color(0.75f, 0.75f, 1.0f);
    draw_tile(
        this->m_character_tiles[static_cast<int>(c->animal())],
        Color(color.r(), color.g(), color.b(), 2.0f / 3.0f),
        p);
}

bool GameDisplay::should_highlight(const Point & p) {
    auto c = this->m_controller;
    return (c->has_highlighted() && p == c->highlighted())
        || c->move_possible(p);
}

void GameDisplay::pick(const QPoint & position) {
    const int BUFFER_SIZE = 32;
    GLuint select_buffer[BUFFER_SIZE];
    GLint hits, viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFFER_SIZE, select_buffer);
    glRenderMode(GL_SELECT);
    this->m_mode = GL_SELECT;
    glInitNames();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(
        static_cast<GLdouble>(position.x()),
        static_cast<GLdouble>(viewport[3])
            - static_cast<GLdouble>(position.y()),
        1.0, 1.0, viewport);
    gluPerspective(
        45.0,
        static_cast<GLdouble>(viewport[2])
            / static_cast<GLdouble>(viewport[3]),
        0.1, 100.0);
    this->paintGL();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    hits = glRenderMode(GL_RENDER);
    this->m_mode = GL_RENDER;
    if(hits == 0)
        this->m_controller->clear_highlighted();
    else {
        GLuint name = select_buffer[3];
        Point p(name % 7, name / 7);
        if(p != this->m_controller->highlighted()) {
            this->m_controller->set_highlighted(p);
            this->update();
        }
    }
}

/*virtual*/
void GameDisplay::mouseMoveEvent(QMouseEvent * event) {
    this->pick(event->pos());
}

/*virtual*/
void GameDisplay::mousePressEvent(QMouseEvent * /*event*/) {
    auto highlighted = this->m_controller->highlighted();
    if(highlighted == Point(-1, -1))
        return;
    this->m_controller->click(highlighted);
    this->update();
}

/*virtual*/
void GameDisplay::mouseReleaseEvent(QMouseEvent * /*event*/) {
    auto highlighted = this->m_controller->highlighted();
    if(highlighted == Point(-1, -1))
        return;
    this->m_controller->click(highlighted);
    this->update();
}

void GameDisplay::send_quit_message() {
    if(this->m_controller->state() == Game::State::WAITING_PLAYER)
        this->m_controller->send_quit_message();
}

void GameDisplay::update_display() {
    this->m_controller->update();
    this->update();
    switch(this->m_controller->state()) {
        case Game::State::VICTORY:
            QMessageBox::information(this, "Victory", "You won.");
            this->window()->close();
            break;
        case Game::State::DEFEAT:
            QMessageBox::information(this, "Defeat", "You lost");
            this->window()->close();
            break;
        case Game::State::EXITING:
            this->window()->close();
            break;
        default:
            break;
    }
}
