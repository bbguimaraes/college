#include <QApplication>
#include <QTimer>

#include "sms/springmasssystem.h"

#include "smscreator.h"
#include "display.h"

void init_simulations(Simulation * simulations, SpringMassSystem * systems);
void init_display(Display * display);
void init_sm_system0(SpringMassSystem * system);
void init_sm_system1(SpringMassSystem * system, unsigned int width);
void init_sm_system2(SpringMassSystem * system, unsigned int width);
GLuint load_texture(const std::string & filename);
void move_system(SpringMassSystem * system, Vector v);

int main(int argc, char ** argv) {
    const double UPDATE_RATE = 1.0f / 3000.0f;
    QApplication app(argc, argv);
    SpringMassSystem systems[2];
    Simulation simulations[2];
    init_simulations(simulations, systems);
    for(auto & x : simulations) {
        x.set_paused(true);
        x.set_update_rate(UPDATE_RATE);
    }
    Display display;
    init_display(&display);
    display.set_simulations({&simulations[0], &simulations[1]});
    display.show();
    QTimer timer;
    QObject::connect(
        &timer, &QTimer::timeout,
        [&simulations](){for(auto & x : simulations) x.update();});
    QObject::connect(
        &timer, &QTimer::timeout,
        &display, &Display::updateGL);
    timer.start(1000.0f / 60.0f);
    return app.exec();
}

void init_simulations(Simulation * simulations, SpringMassSystem * systems) {
    const unsigned int S1_W = 10, S2_W = 10;
    init_sm_system1(&systems[0], S1_W),
    init_sm_system2(&systems[1], S2_W),
    move_system(&systems[0], Vector(-10.0f, 0.0f));
    move_system(&systems[1], Vector( 10.0f, 0.0f));
    simulations[0].set_systems({&systems[0]});
    simulations[1].set_systems({&systems[1]});
    simulations[0].set_texturable(true);
    simulations[1].set_texturable(true);
    simulations[0].set_texture_width(S1_W);
    simulations[1].set_texture_width(S2_W);
}

void init_display(Display * display) {
    display->makeCurrent();
    display->set_texture(load_texture("texture.jpg"));
    display->camera()->set_distance(30.0f);
    display->setMouseTracking(true);
    display->resize(800, 600);
}

void init_sm_system0(SpringMassSystem * system) {
    SMSCreator::create_string(
        system,
        Vector(),
        Vector(5.0f, 0.0f, 0.0f),
        10, 1, 1);
    (*system->masses())[0].set_fixed(true);
}

void init_sm_system1(SpringMassSystem * system, unsigned int width) {
    SMSCreator::create_flag(
        system,
        Vector(-5.0f, 10.0f), Vector(5.0f, 0.0f),
        width, 5,
        1.0f, 30.0f);
    (*system->masses())[0].set_fixed(true);
    (*system->masses())[width - 1].set_fixed(true);
}

void init_sm_system2(SpringMassSystem * system, unsigned int width) {
    SMSCreator::create_crossed_flag(
        system,
        Vector(-5.0f, 10.0f), Vector(5.0f, 0.0f),
        width, 5,
        1.0f, 10.0f);
    (*system->masses())[0].set_fixed(true);
    (*system->masses())[width - 1].set_fixed(true);
}

GLuint load_texture(const std::string & filename) {
    auto image = QGLWidget::convertToGLFormat(QImage(filename.c_str()));
    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA,
        GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return ret;
}

void move_system(SpringMassSystem * system, Vector v) {
    for(auto & x : *system->masses())
        x.set_position(x.position() + v);
}
