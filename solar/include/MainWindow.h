#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <vector>
#include <string>

#include <SolarSystem.h>
#include <Planet.h>

class QLabel;
class QPushButton;
class QSlider;
class QSpinBox;

class GLDisplay;
class WebSearchWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget * parent = nullptr);

    public slots:
        void set_fps(int fps);
        void set_shininess(int shininess);

        void highlight_panel(Planet * planet);
        void show_planet(Planet * planet);
        void center_camera();

        void load_started();
        void load_finished(bool ok);

        void load_solar_system_file();
        void load_solar_system_file(const std::string & filename);

        void opengl_error(GLenum error);

    private:
        void init();
        static OpenGLTexture * load_texture(const std::string & filename);

        std::vector<Planet> m_planets;
        SolarSystem m_solar_system;

        GLDisplay * m_display;
        QTimer m_display_timer;

        QLabel * m_url_label;
        std::vector<WebSearchWidget *> m_websearch_widgets;

        QDockWidget * m_controls_dock;
        QDockWidget * m_options_dock;
        QDockWidget * m_content_dock;

        QPushButton * m_pause_button;
        QPushButton * m_slow_button;
        QPushButton * m_center_camera_button;
        QPushButton * m_ambient_button;
        QPushButton * m_diffuse_button;
        QPushButton * m_specular_button;
        QPushButton * m_local_viewer_button;
        QPushButton * m_line_smooth_button;
        QPushButton * m_perspective_corr_button;
        QPushButton * m_texture_button;
        QSlider * m_shininess_slider;
        QSpinBox * m_fps_spin;
};

#endif // MAIN_WINDOW_H
