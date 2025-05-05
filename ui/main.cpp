#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CP::UI::MainWindow window;
    window.show();

    return app.exec();
}
