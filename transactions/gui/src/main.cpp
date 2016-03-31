#include <QApplication>

#include "MainWindow.h"

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    std::srand(time(0));
    MainWindow window;
    window.resize(800, 600);
    window.show();
    return app.exec();
}
