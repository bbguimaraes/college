#include <QApplication>
#include <QTimer>

#include "sms/springmasssystem.h"

#include "smscreator.h"
#include "display.h"

void init_simulations(
    std::vector<Simulation> * simulations,
    std::vector<SpringMassSystem> * systems);
void init_display(Display * display);
void init_sm_system0(SpringMassSystem * system, bool fixed_end);
void init_sm_system1(
    SpringMassSystem * system, unsigned int width, unsigned int height);
void init_sm_system2(SpringMassSystem * system, unsigned int width);
void init_sm_system3(SpringMassSystem * system, unsigned int side);
GLuint load_texture(const std::string & filename);
void move_system(SpringMassSystem * system, Vector v);

void update_simulations(
    std::vector<Simulation> * simulations, Display * display);

int main(int argc, char ** argv) {
    const double UPDATE_RATE = 1.0f / 3000.0f;
    QApplication app(argc, argv);
    auto systems = std::vector<SpringMassSystem>(6);
    auto simulations = std::vector<Simulation>(4);
    init_simulations(&simulations, &systems);
    for(auto & x : simulations) {
        x.set_paused(true);
        x.set_visible(false);
        x.set_update_rate(UPDATE_RATE);
    }
    Display display;
    init_display(&display);
    display.set_simulations(
        {&simulations[0], &simulations[1], &simulations[2], &simulations[3]});
    display.show();
    QTimer timer;
    QObject::connect(
        &timer, &QTimer::timeout,
        [&simulations, &display]() {
            for(unsigned int i = 0; i < 2; ++i)
                update_simulations(&simulations, &display);
        });
    QObject::connect(
        &timer, &QTimer::timeout,
        &display, &Display::updateGL);
    timer.start(1000.0f / 60.0f);
    return app.exec();
}

void update_simulations(
        std::vector<Simulation> * simulations, Display * display) {
    const auto W = Vector(0.0f, 0.0f, -0.2f)
        * (static_cast<float>(std::rand()) / RAND_MAX);
    for(auto & simulation : *simulations) {
        const auto F = simulation.wind() ? W : Vector();
        for(auto system : *simulation.systems())
            for(auto & mass : *system->masses()) {
                auto f = F;
                if(&mass == display->selected())
                    f = f + display->selected_force();
                mass.set_force(f);
            }
        simulation.update();
    }
}

void init_simulations(
        std::vector<Simulation> * simulations,
        std::vector<SpringMassSystem> * systems) {
    const unsigned int S1_W = 14, S1_H = 11, S2_W = 11;
    init_sm_system0(&(*systems)[0], false),
    init_sm_system0(&(*systems)[1], true),
    init_sm_system1(&(*systems)[2], S1_W, S1_H),
    init_sm_system2(&(*systems)[3], S2_W),
    init_sm_system3(&(*systems)[4], 5),
    init_sm_system3(&(*systems)[5], 10),
    move_system(&(*systems)[0], Vector(  0.0f,  5.0f));
    move_system(&(*systems)[1], Vector( -1.0f, 10.0f));
    move_system(&(*systems)[2], Vector(-15.0f, -5.0f));
    move_system(&(*systems)[3], Vector(  5.0f, -5.0f));
    move_system(&(*systems)[4], Vector(-15.0f, -10.0f, -5.0f));
    move_system(&(*systems)[5], Vector(  5.0f, -10.0f, -5.0f));
    (*simulations)[0].set_systems({&(*systems)[0], &(*systems)[1]});
    (*simulations)[1].set_systems({&(*systems)[2]});
    (*simulations)[2].set_systems({&(*systems)[3]});
    (*simulations)[3].set_systems({&(*systems)[4], &(*systems)[5]});
    (*simulations)[1].set_texturable(true);
    (*simulations)[2].set_texturable(true);
    (*simulations)[1].set_texture_width(S1_W);
    (*simulations)[2].set_texture_width(S2_W);
    (*simulations)[1].set_texture_height(S1_H);
    (*simulations)[2].set_texture_height(S2_W);
}

void init_display(Display * display) {
    display->makeCurrent();
    display->set_texture(load_texture("texture.jpg"));
    display->camera()->set_distance(30.0f);
    display->setMouseTracking(true);
    display->resize(800, 600);
}

void init_sm_system0(SpringMassSystem * system, bool fixed_end) {
    SMSCreator::create_string(
        system,
        Vector(),
        Vector(2.0f, 0.0f, 0.0f),
        10, 0.1f, 1);
    (*system->masses())[0].set_fixed(true);
    (*system->masses())[9].set_fixed(fixed_end);
}

void init_sm_system1(
        SpringMassSystem * system, unsigned int width, unsigned int height) {
    SMSCreator::create_flag(
        system,
        Vector(0.0f, 0.0f), Vector(10.0f, 10.0f),
        width, height,
        2.0f, 20.0f);
    (*system->masses())[width * (height - 1)].set_fixed(true);
    (*system->masses())[width *  height - 1 ].set_fixed(true);
}

void init_sm_system2(SpringMassSystem * system, unsigned int width) {
    SMSCreator::create_crossed_flag(
        system,
        Vector(0.0f, 0.0f), Vector(10.0f, 10.0f),
        width, width,
        1.0f, 10.0f);
    (*system->masses())[width * (width - 1)].set_fixed(true);
    (*system->masses())[width *  width - 1 ].set_fixed(true);
}

void init_sm_system3(SpringMassSystem * system, unsigned int side) {
    const float SIZE = 10.0f;
    SMSCreator::create_crossed_flag(
        system,
        Vector(0.0f, SIZE), Vector(SIZE, 0.0f),
        side, side,
        1.0f, 10.0f);
    for(auto & x : *system->masses()) {
        auto p = x.position();
        if(!p.x() || !p.y() || p.x() == SIZE || p.y() == SIZE)
            x.set_fixed(true);
        x.set_position(Vector(p.x(), p.z(), p.y()));
    }
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
