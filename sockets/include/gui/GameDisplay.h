#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#include <vector>

#include <QGLWidget>

class Character;
class Color;
class GameController;
class Point;
class Texture;
class Tile;

class GameDisplay : public QGLWidget {
    Q_OBJECT

    public:
        GameDisplay(QWidget * parent = nullptr);
        ~GameDisplay();

        void set_game_controller(GameController * c) {m_controller = c;}
        void send_quit_message();

    public slots:
        void update_display();

    protected:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);

    private:
        struct Texture {
            enum {
                RAT, CAT, DOG, WOLF, PANTHER, TIGER, LION, ELEPHANT,
                GRASS, WATER, TRAP, DEN,
            };
        };

        std::vector<::Texture *> load_textures();
        std::vector<Tile *> create_tiles();
        void pick(const QPoint & position);
        void draw_grid();
        void draw_objects();
        void draw_tile(const Tile * t, const Color & c, const Point & p);
        void draw_character(const Character * t, const Point & p);
        bool should_highlight(const Point & p);

        static const float GRID_SIZE;
        static const float WIDTH;
        static const float HEIGHT;
        GameController * m_controller;
        GLenum m_mode;
        float m_angles[3];
        std::vector<::Texture *> m_textures;
        std::vector<Tile *> m_tiles;
        std::vector<Tile *> m_character_tiles;
};

#endif
