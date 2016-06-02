#include <MainWindow.h>

#include <fstream>
#include <exception>

#include <QDockWidget>
#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>

#include "Camera.h"
#include "GLDisplay.h"
#include "OpenGLTexture.h"
#include "WebSearchWidget.h"

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent) {
    this->init();
    this->m_shininess_slider->setValue(this->m_display->shininess());
    this->m_options_dock->hide();
    this->m_controls_dock->hide();
    this->m_pause_button->setChecked(this->m_display->paused());
    this->m_slow_button->setChecked(this->m_display->slow());
    this->m_ambient_button->setChecked(this->m_display->ambient());
    this->m_diffuse_button->setChecked(this->m_display->diffuse());
    this->m_specular_button->setChecked(this->m_display->specular());
    this->m_local_viewer_button->setChecked(this->m_display->local_viewer());
    this->m_perspective_corr_button->setChecked(
        this->m_display->perspective_corr());
    this->m_texture_button->setChecked(this->m_display->texture());
    connect(
        &this->m_display_timer, SIGNAL(timeout()),
        this->m_display, SLOT(update()));
    connect(
        this->m_display, SIGNAL(planet_highlighted(Planet *)),
        this, SLOT(highlight_panel(Planet *)));
    connect(
        this->m_display, SIGNAL(planet_selected(Planet *)),
        this, SLOT(show_planet(Planet *)));
    connect(
        this->m_display, SIGNAL(opengl_error(GLenum)),
        this, SLOT(opengl_error(GLenum)));
    connect(
        this->m_pause_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_paused()));
    connect(
        this->m_slow_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_slow()));
    connect(
        this->m_center_camera_button, SIGNAL(clicked()),
        this, SLOT(center_camera()));
    connect(
        this->m_ambient_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_ambient()));
    connect(
        this->m_diffuse_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_diffuse()));
    connect(
        this->m_specular_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_specular()));
    connect(
        this->m_local_viewer_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_local_viewer()));
    connect(
        this->m_line_smooth_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_line_smooth()));
    connect(
        this->m_perspective_corr_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_perspective_corr()));
    connect(
        this->m_texture_button, SIGNAL(clicked()),
        this->m_display, SLOT(toggle_texture()));
    connect(
        this->m_shininess_slider, SIGNAL(valueChanged(int)),
        this->m_display, SLOT(set_shininess(int)));
    connect(
        this->m_fps_spin, SIGNAL(valueChanged(int)),
        this, SLOT(set_fps(int)));
    const float RATE = 1000.0f / this->m_fps_spin->value();
    this->m_display->set_update_rate(RATE);
    this->m_display_timer.start(RATE);
}

void MainWindow::init() {
    this->statusBar()->show();
    this->m_display = new GLDisplay(this);
    this->m_display->setMouseTracking(true);
    this->m_url_label = new QLabel("Click on a planet");
    QTextEdit * content_edit = new QTextEdit;
    content_edit->setReadOnly(true);
    content_edit->setText(
        "This program simulates the solar system. It is scaled so that all"
        " planets can be seen. The planets appear 500000x, except for the sun"
        " which is scaled 25000x. The distance between the planets is scaled"
        " down 5 times.\n"
        "Clicking on a planet shows content of various sites on the docks."
        " Search engines can be toggled on the View menu.");
    this->m_websearch_widgets.push_back(new WebSearchWidget);
    this->m_websearch_widgets[0]->setWindowTitle("Wikipedia");
    this->m_websearch_widgets[0]->set_engine("http://en.wikipedia.org/wiki/");
    this->m_websearch_widgets.push_back(new WebSearchWidget);
    this->m_websearch_widgets[1]->setWindowTitle("Google");
    this->m_websearch_widgets[1]->set_engine("http://google.com/search?q=");
    this->m_websearch_widgets.push_back(new WebSearchWidget);
    this->m_websearch_widgets[2]->setWindowTitle("Google images");
    this->m_websearch_widgets[2]->set_engine(
        "http://google.com/search?tbm=isch&q=");
    this->m_websearch_widgets.push_back(new WebSearchWidget);
    this->m_websearch_widgets[3]->setWindowTitle("Youtube");
    this->m_websearch_widgets[3]->set_engine(
        "http://www.youtube.com/results?search_query=");
    QFrame * button_panel = new QFrame(this);
    button_panel->setAutoFillBackground(true);
    button_panel->setPalette(QPalette(Qt::white));
    button_panel->setFrameStyle(QFrame::StyledPanel);
    this->m_pause_button = new QPushButton("Pause");
    this->m_slow_button = new QPushButton("Slow");
    this->m_center_camera_button = new QPushButton("Center camera");
    this->m_ambient_button = new QPushButton("Ambient");
    this->m_diffuse_button = new QPushButton("Diffuse");
    this->m_specular_button = new QPushButton("Specular");
    this->m_local_viewer_button = new QPushButton("Local viewer");
    this->m_line_smooth_button = new QPushButton("Line smooth");
    this->m_perspective_corr_button = new QPushButton("Perspective corr");
    this->m_texture_button = new QPushButton("Texture");
    this->m_pause_button->setCheckable(true);
    this->m_slow_button->setCheckable(true);
    this->m_ambient_button->setCheckable(true);
    this->m_diffuse_button->setCheckable(true);
    this->m_specular_button->setCheckable(true);
    this->m_local_viewer_button->setCheckable(true);
    this->m_line_smooth_button->setCheckable(true);
    this->m_perspective_corr_button->setCheckable(true);
    this->m_texture_button->setCheckable(true);
    this->m_texture_button->setChecked(true);
    this->m_shininess_slider = new QSlider(Qt::Horizontal, button_panel);
    this->m_fps_spin = new QSpinBox(button_panel);
    this->m_shininess_slider->setMinimum(0);
    this->m_shininess_slider->setMaximum(128);
    this->m_fps_spin->setMinimum(0);
    this->m_fps_spin->setMaximum(120);
    this->m_fps_spin->setValue(30);
    QTextEdit * controls_edit = new QTextEdit;
    controls_edit->setReadOnly(true);
    controls_edit->setText(
        "Mouse:\n\n"
        "Left-click: select planet.\n"
        "Right-click drag: rotate camera\n"
        "Wheel: zoom\n\n\n"
        "Keyboard:\n\n"
        "WASD: move camera on the XZ plane\n"
        "Up and down arrows: zoom\n"
        "Left and right arrows: rotate camera");
    QVBoxLayout * web_view_layout = new QVBoxLayout;
    web_view_layout->addWidget(this->m_url_label, 0);
    web_view_layout->addWidget(content_edit, 0);
    QVBoxLayout * button_panel_layout = new QVBoxLayout(button_panel);
    button_panel->setLayout(button_panel_layout);
    button_panel_layout->addWidget(this->m_pause_button);
    button_panel_layout->addWidget(this->m_slow_button);
    button_panel_layout->addWidget(this->m_center_camera_button);
    button_panel_layout->addWidget(this->m_ambient_button);
    button_panel_layout->addWidget(this->m_diffuse_button);
    button_panel_layout->addWidget(this->m_specular_button);
    button_panel_layout->addWidget(this->m_local_viewer_button);
    button_panel_layout->addWidget(this->m_line_smooth_button);
    button_panel_layout->addWidget(this->m_perspective_corr_button);
    button_panel_layout->addWidget(this->m_texture_button);
    button_panel_layout->addWidget(new QLabel("Shininess"));
    button_panel_layout->addWidget(this->m_shininess_slider);
    button_panel_layout->addWidget(new QLabel("FPS"));
    button_panel_layout->addWidget(this->m_fps_spin);
    button_panel_layout->addStretch(1);
    for(int i = 0; i < button_panel->layout()->count(); ++i) {
        auto t = button_panel->layout()->itemAt(i);
        if(t->widget())
            t->widget()->installEventFilter(this);
    }
    this->setDockNestingEnabled(true);
    this->m_content_dock = new QDockWidget(this);
    QWidget * content_widget = new QWidget;
    content_widget->setLayout(web_view_layout);
    this->m_content_dock->setWidget(content_widget);
    this->m_content_dock->setWindowTitle("Content");
    this->addDockWidget(Qt::RightDockWidgetArea, this->m_content_dock);
    this->m_options_dock = new QDockWidget(this);
    this->m_options_dock->setWidget(button_panel);
    this->m_options_dock->setWindowTitle("Options");
    addDockWidget(Qt::LeftDockWidgetArea, this->m_options_dock);
    this->m_controls_dock = new QDockWidget(this);
    this->m_controls_dock->setWidget(controls_edit);
    this->m_controls_dock->setWindowTitle("Controls");
    addDockWidget(Qt::LeftDockWidgetArea, this->m_controls_dock);
    QFrame * central_frame = new QFrame(this);
    central_frame->setAutoFillBackground(true);
    central_frame->setFrameStyle(QFrame::StyledPanel);
    central_frame->setPalette(Qt::white);
    QGridLayout * central_layout = new QGridLayout(central_frame);
    central_layout->setSpacing(10);
    central_layout->addWidget(this->m_display);
    this->setCentralWidget(central_frame);
    this->m_display->set_solar_system(&this->m_solar_system);
    this->m_display->installEventFilter(this);
    this->m_display->makeCurrent();
    auto camera = this->m_display->camera();
    camera->set_x_angle(10.0f);
    camera->set_y_angle(180.0f);
    camera->set_distance(1.0f);
    QAction * load_file_action = new QAction("&Load", this);
    QAction * quit_action = new QAction("&Quit", this);
    QAction * content_view_action = new QAction("&Content", this);
    QAction * options_view_action = new QAction("&Options", this);
    QAction * controls_view_action = new QAction("&Controls", this);
    QAction * show_all_action = new QAction("&Show all", this);
    QAction * hide_all_action = new QAction("&Hide all", this);
    content_view_action->setCheckable(true);
    content_view_action->setChecked(true);
    options_view_action->setCheckable(true);
    options_view_action->setChecked(false);
    controls_view_action->setCheckable(true);
    controls_view_action->setChecked(false);
    connect(
        quit_action, SIGNAL(triggered()),
        this, SLOT(close()));
    connect(
        load_file_action, SIGNAL(triggered()),
        this, SLOT(load_solar_system_file()));
    connect(
        content_view_action, SIGNAL(toggled(bool)),
        this->m_content_dock, SLOT(setVisible(bool)));
    connect(
        this->m_content_dock, SIGNAL(visibilityChanged(bool)),
        content_view_action, SLOT(setChecked(bool)));
    connect(
        options_view_action, SIGNAL(toggled(bool)),
        this->m_options_dock, SLOT(setVisible(bool)));
    connect(
        this->m_options_dock, SIGNAL(visibilityChanged(bool)),
        options_view_action, SLOT(setChecked(bool)));
    connect(
        controls_view_action, SIGNAL(toggled(bool)),
        this->m_controls_dock, SLOT(setVisible(bool)));
    connect(
        this->m_controls_dock, SIGNAL(visibilityChanged(bool)),
        controls_view_action, SLOT(setChecked(bool)));
    QMenu * file_menu = this->menuBar()->addMenu("&File");
    QMenu * view_menu = this->menuBar()->addMenu("&View");
    file_menu->addAction(load_file_action);
    file_menu->addAction(quit_action);
    view_menu->addAction(options_view_action);
    view_menu->addAction(controls_view_action);
    view_menu->addSeparator();
    view_menu->addAction(show_all_action);
    view_menu->addAction(hide_all_action);
    view_menu->addAction(content_view_action);
    connect(
        show_all_action, SIGNAL(triggered()),
        this->m_content_dock, SLOT(show()));
    connect(
        hide_all_action, SIGNAL(triggered()),
        this->m_content_dock, SLOT(hide()));
    for(WebSearchWidget * wsw : this->m_websearch_widgets) {
        QWidget * w = new QWidget;
        QVBoxLayout * vbl = new QVBoxLayout;
        QProgressBar * pb = new QProgressBar;
        vbl->addWidget(wsw);
        vbl->addWidget(pb);
        w->setLayout(vbl);
        pb->setVisible(false);
        connect(wsw, SIGNAL(loadStarted()), pb, SLOT(show()));
        connect(wsw, SIGNAL(loadFinished(bool)), pb, SLOT(hide()));
        connect(wsw, SIGNAL(loadProgress(int)), pb, SLOT(setValue(int)));
        QDockWidget * dw = new QDockWidget(this);
        dw->setWidget(w);
        dw->setWindowTitle(wsw->windowTitle());
        this->addDockWidget(Qt::RightDockWidgetArea, dw);
        QAction * a = new QAction("&" + wsw->windowTitle(), this);
        connect(a, SIGNAL(toggled(bool)), dw, SLOT(setVisible(bool)));
        connect(
            dw, SIGNAL(visibilityChanged(bool)), a, SLOT(setChecked(bool)));
        a->setCheckable(true);
        a->setChecked(true);
        view_menu->addAction(a);
        connect(show_all_action, SIGNAL(triggered()), dw, SLOT(show()));
        connect(hide_all_action, SIGNAL(triggered()), dw, SLOT(hide()));
    }
    this->m_display->setFocus();
}

/*static*/
OpenGLTexture * MainWindow::load_texture(const std::string & filename) {
    QImage img(filename.c_str());
    return new OpenGLTexture(
        QGLWidget::convertToGLFormat(img).bits(),
        GL_RGBA,
        img.width(),
        img.height());
}

void MainWindow::set_fps(int fps) {
    this->m_display->set_update_rate(1000.0f / fps);
    this->m_display_timer.start(1000.0f / fps);
}

void MainWindow::set_shininess(int shininess) {
    this->m_display->set_shininess(shininess);
}

void MainWindow::load_solar_system_file() {
    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty() == false)
        load_solar_system_file(filename.toStdString());
}

void MainWindow::load_solar_system_file(const std::string & filename) {
    std::ifstream reader(filename);
    std::string str;
    try {
        while(!reader.eof() && (reader.peek() == '\n' || reader.peek() == '#'))
            getline(reader, str);
        unsigned int size;
        reader >> size;
        std::vector<Planet> planets;
        planets.reserve(size);
        for(unsigned int i = 0; i < size; ++i) {
            std::string name, texture;
            float distance, speed, radius;
            bool star;
            reader >> name >> texture >> distance >> speed >> radius >> star;
            planets.push_back(
                Planet(
                    name,
                    distance / 5.0f,
                    speed ? 365.0f / speed / 100.0f : 0,
                    radius
                        / Planet::AU_TO_KM
                        * 500000.0f
                        / (name == "Sun" ? 20.0f : 1.0f),
                    this->load_texture(texture),
                    star));
        }
        m_planets = planets;
        std::vector<Planet *> planetPointers;
        for(Planet & p : m_planets)
            planetPointers.push_back(&p);
        m_solar_system.set_planets(planetPointers);
    } catch(std::exception & ex) {
        QMessageBox::critical(
            this,
            "Could not read file " + QString::fromStdString(filename),
            QString(ex.what()));
    }
}

void MainWindow::highlight_panel(Planet * planet) {
    if(planet) {
        QString name = QString::fromStdString(planet->name());
        this->m_url_label->setText(
            name + " selected. Click for more information.");
        this->statusBar()->showMessage(name);
    } else {
        this->m_url_label->setText("Click on a planet.");
        this->statusBar()->showMessage("");
    }
    this->m_display->set_selected(planet);
}

void MainWindow::show_planet(Planet * planet) {
    if(!planet)
        return;
    for(WebSearchWidget * wsw : m_websearch_widgets)
        if(wsw->isVisible())
            wsw->search(planet->name());
}

void MainWindow::center_camera() {
    Camera * camera = this->m_display->camera();
    camera->set_x_angle(45.0f);
    camera->set_y_angle(0.0f);
    camera->set_z_angle(0.0f);
}

void MainWindow::load_started() {
    this->m_url_label->setText("Loading...");
}

void MainWindow::load_finished(bool ok) {
    if(!ok)
        QMessageBox::critical(
            this, "Could not open page", "Error while loading page");
}

void MainWindow::opengl_error(GLenum error) {
    const GLubyte * str = gluErrorString(error);
    QMessageBox::critical(
        this, "OpenGL error", reinterpret_cast<const char *>(str));
    this->close();
}
