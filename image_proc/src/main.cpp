#include <QApplication>

#include "include/mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.move(0, 0);
    window.show();
    return app.exec();
}
