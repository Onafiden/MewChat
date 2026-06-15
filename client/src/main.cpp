#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "ui/chatwindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MewChat window;
    window.setWindowTitle("MewChat");
    window.resize(800, 600);
    
    QFile styleFile("../resources/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    }
    

    window.show();

    return app.exec();
}