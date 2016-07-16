#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>

#include <QGLWidget>
#include <QTime>

#include "sms/vector.h"
#include "camera.h"
#include "simulation.h"

class GLUquadric;

class Mass;
class SpringMassSystem;

class Display : public QGLWidget {
    Q_OBJECT

    const unsigned int GRID_SIZE = 100;
    const float AXES_HEIGHT = 5.0f;
    const unsigned int AXES_SLICES = 32;
    const float SELECT_RATE_LIMIT = 1000.0f / 5.0f;

    public:
        Display(QWidget * parent = nullptr);
        ~Display();

        Camera * camera() {return &this->m_camera;}

        bool draw_grid() const {return this->m_draw_grid;}
        bool draw_axes() const {return this->m_draw_axes;}
        void set_simulations(const std::vector<Simulation *> & v)
            {this->m_simulations = v;}
        void set_texture(GLuint t) {this->m_texture = t;}
        void set_draw_grid(bool d) {this->m_draw_grid = d;}
        void set_draw_axes(bool d) {this->m_draw_axes = d;}

    public slots:
        void update_systems();

    protected:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

        virtual void keyPressEvent(QKeyEvent * event);
        virtual void keyReleaseEvent(QKeyEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void wheelEvent(QWheelEvent * event);

    private:
        void select(Vector click, bool rate_limit);
        void draw_simulation(Simulation * s);
        void update_fps();

        std::vector<Simulation *> m_simulations;
        Mass * m_selected;
        Camera m_camera;
        GLuint m_texture;
        GLUquadric * m_quadric;

        QTime m_last_select;
        QTime m_last_frame;
        unsigned int m_frame_count;
        unsigned int m_fps;

        bool m_ctrl_key_down;
        Vector m_click_position;
        Vector m_last_position;

        bool m_draw_grid;
        bool m_draw_axes;
};

#endif // DISPLAY_H
