#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include "MainWindow.h"
int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("translations/qt_" + QLocale::system().name());
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("translations/" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    MainWindow w;
    w.show();
    return app.exec();
}
