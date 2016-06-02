#include <QApplication>

#include "MainWindow.h"

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    MainWindow w;
    w.load_solar_system_file("planets.txt");
    w.resize(1365, 768);
    w.show();
    w.showMaximized();
    return app.exec();
}
