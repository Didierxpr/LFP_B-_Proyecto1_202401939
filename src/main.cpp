#include <QApplication>
#include "gui/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
    std::cout << "App iniciando..." << std::endl;
}
