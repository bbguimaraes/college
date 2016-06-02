#ifndef GL_DISPLAY_H
#define GL_DISPLAY_H

#include <set>

#include <QGLWidget>
#include <QTime>

#include "Camera.h"
#include "Vector.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class OpenGLTexture;

class SolarSystem;
class Planet;

class GLDisplay : public QGLWidget {
    Q_OBJECT

    public:
        GLDisplay(QWidget * parent = nullptr);

        Camera * camera() {return &this->m_camera;}
        bool paused() const {return this->m_paused;}
        bool slow() const {return this->m_slow;}
        bool ambient() const {return this->m_ambient;}
        bool diffuse() const {return this->m_diffuse;}
        bool specular() const {return this->m_specular;}
        int shininess() const {return this->m_shininess;}
        bool local_viewer() const {return this->m_local_viewer;}
        bool perspective_corr() const {return this->m_perspective_corr;}
        bool texture() const {return this->m_texture;}

        void set_solar_system(SolarSystem * ss) {this->m_solar_system = ss;}

    public slots:
        virtual void update();
        void set_update_rate(float ur) {this->m_update_rate = ur;}
        void set_selected(Planet * p) {this->m_selected = p;}

        void toggle_paused() {this->m_paused = !m_paused;}
        void toggle_slow() {this->m_slow = !m_slow;}
        void toggle_ambient();
        void toggle_diffuse();
        void toggle_specular();
        void set_shininess(int shininess);
        void toggle_local_viewer() {this->m_local_viewer = !m_local_viewer;}
        void toggle_line_smooth();
        void toggle_perspective_corr();
        void toggle_texture() {this->m_texture = !this->m_texture;}

    signals:
        void planet_highlighted(Planet * planet);
        void planet_selected(Planet * planet);
        void opengl_error(GLenum error);

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
        GLDisplay(GLDisplay &) = delete;
        GLDisplay & operator =(GLDisplay &) = delete;

        static GLuint create_planet_list(int slices);

        bool key_pressed(int key) const;
        void check_keys();
        void draw_background() const;
        void draw_system(GLenum mode) const;
        void pick(const Vector & pos);
        void set_camera();

        float m_update_rate;
        float m_zoom_speed;
        OpenGLTexture * m_background;

        SolarSystem * m_solar_system;
        Planet * m_selected;
        Camera m_camera;
        std::set<int> m_keys_pressed;

        GLuint m_planet_list;
        GLuint m_planet_select_list;
        Vector m_last_position;

        bool m_paused;
        bool m_slow;
        bool m_ambient;
        bool m_diffuse;
        bool m_specular;
        int m_shininess;
        bool m_local_viewer;
        bool m_line_smooth;
        bool m_perspective_corr;
        bool m_texture;

        QTime m_last_frame;
        unsigned int m_frame_count;
};

#endif // GL_DISPLAY_H
