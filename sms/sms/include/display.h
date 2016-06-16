#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>

#include <QGLWidget>
#include <QTime>

#include "sms/vector.h"
#include "camera.h"

class GLUquadric;

class Mass;
class SpringMassSystem;

class Display : public QGLWidget {
    Q_OBJECT

    public:
        Display(QWidget * parent = nullptr);
        ~Display();

        float update_rate() const {return this->m_update_rate;}
        Camera * camera() {return &this->m_camera;}
        bool paused() const {return this->m_paused;}
        bool textured() const {return this->m_textured;}

        void set_systems(const std::vector<SpringMassSystem *> & v)
            {this->m_systems = v;}
        void set_update_rate(float r) {this->m_update_rate = r;}
        void set_texture(GLuint t) {this->m_texture = t;}
        void set_paused(bool p) {this->m_paused = p;}
        void set_draw_grid(bool d) {this->m_draw_grid = d;}
        void set_grid_size(int s) {this->m_grid_size = s;}
        void set_draw_axes(bool d) {this->m_draw_axes = d;}
        void set_draw_masses(bool d) {this->m_draw_masses = d;}
        void set_textured(bool t) {this->m_textured = t;}

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
        void draw_system(SpringMassSystem * system);
        void draw_springs_non_textured(SpringMassSystem * system);
        void draw_springs_textured(SpringMassSystem * system);
        void draw_masses(SpringMassSystem * system, GLenum mode);
        void draw_hud();
        void draw_number(unsigned int n);
        void update_fps();

        std::vector<SpringMassSystem *> m_systems;
        Mass * m_selected;
        float m_update_rate;
        Camera m_camera;
        GLuint m_texture;
        GLUquadric * m_quadric;

        QTime m_last_frame;
        unsigned int m_frame_count;
        unsigned int m_fps;

        bool m_ctrl_key_down;
        Vector m_click_position;
        Vector m_last_position;

        bool m_paused;
        bool m_draw_grid;
        bool m_draw_axes;
        bool m_draw_masses;
        bool m_textured;
        int m_grid_size;
};

#endif // DISPLAY_H
