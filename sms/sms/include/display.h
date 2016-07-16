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

    public:
        Display(QWidget * parent = nullptr);
        ~Display();

        Camera * camera() {return &this->m_camera;}

        void set_simulations(const std::vector<Simulation *> & v)
            {this->m_simulations = v;}
        void set_texture(GLuint t) {this->m_texture = t;}
        void set_draw_grid(bool d) {this->m_draw_grid = d;}
        void set_grid_size(int s) {this->m_grid_size = s;}
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
        void set_camera();
        void select(Vector click);
        void draw_grid();
        void draw_axes();
        void draw_axis(float height);
        void draw_circle(float radius);
        void draw_simulation(Simulation * s);
        void draw_springs_non_textured(const SpringMassSystem * system);
        void draw_springs_textured(SpringMassSystem * system);
        void draw_masses(const SpringMassSystem * system, GLenum mode);
        void draw_hud();
        void draw_number(unsigned int n);
        void update_fps();

        std::vector<Simulation *> m_simulations;
        Mass * m_selected;
        Camera m_camera;
        GLuint m_texture;
        GLUquadric * m_quadric;

        QTime m_last_frame;
        unsigned int m_frame_count;
        unsigned int m_fps;

        bool m_ctrl_key_down;
        Vector m_click_position;
        Vector m_last_position;

        bool m_draw_grid;
        bool m_draw_axes;
        int m_grid_size;
};

#endif // DISPLAY_H
