#include <sstream>

#include <QApplication>
#include <QTimer>

#include "sms/mass.h"
#include "sms/springmasssystem.h"

#include "smscreator.h"
#include "display.h"

void init_sm_system0(SpringMassSystem * system);
void init_sm_system1(SpringMassSystem * system);
void init_sm_system2(SpringMassSystem * system);
void init_sm_system3(SpringMassSystem * system);
GLuint load_texture(const std::string & filename);

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    SpringMassSystem sms0, sms1;
    init_sm_system3(&sms0);
    init_sm_system3(&sms1);
    for(auto mass : sms0.masses())
        mass->set_position(mass->position() + Vector(-10.0f));
    for(auto mass : sms1.masses())
        mass->set_position(mass->position() + Vector(10.0f));
    Display display;
    display.makeCurrent();
    display.set_texture(load_texture("texture.jpg"));
    display.set_systems({&sms0, &sms1});
    display.set_update_rate(1.0f / 5000.0f);
    display.set_paused(true);
    display.set_textured(false);
    display.camera()->set_distance(30.0f);
    display.setMouseTracking(true);
    display.resize(800, 600);
    display.show();
    QTimer display_timer, simulation_timer;
    QObject::connect(
        &display_timer, SIGNAL(timeout()),
        &display, SLOT(update_systems()));
    QObject::connect(
        &simulation_timer, SIGNAL(timeout()),
        &display, SLOT(update()));
    display_timer.start(1.0f / 60.0f);
    simulation_timer.start(display.update_rate());
    return app.exec();
}

void init_sm_system0(SpringMassSystem * system) {
    std::stringstream input(
        "# begin\n"
        "0.0 0.0 0.0\n"
        "# end\n"
        "10.0 0.0 0.0\n"
        "# number of Masses\n"
        "11\n"
        "# total mass\n"
        "0.1\n"
        "# spring k constant\n"
        "0.25\n");
    SMSCreator::read_from_file(system, &input);
    system->masses()[0]->set_fixed(true);
}

void init_sm_system1(SpringMassSystem * system) {
    SMSCreator::create_string(
        system,
        Vector(),
        Vector(5.0f, 0.0f, 0.0f),
        10, 1, 1);
    system->masses()[0]->set_fixed(true);
}

void init_sm_system2(SpringMassSystem * system) {
    const unsigned int W = 10;
    SMSCreator::create_flag(
        system,
        Vector(-5.0f, 10.0f), Vector(5.0f, 0.0f),
        W, 5,
        1.0f, 10.0f);
    system->masses()[0]->set_fixed(true);
    system->masses()[W - 1]->set_fixed(true);
}

void init_sm_system3(SpringMassSystem * system) {
    const unsigned int W = 10;
    SMSCreator::create_crossed_flag(
        system,
        Vector(-5.0f, 10.0f), Vector(5.0f, 0.0f),
        W, 5,
        1.0f, 10.0f);
    system->masses()[0]->set_fixed(true);
    system->masses()[W - 1]->set_fixed(true);
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