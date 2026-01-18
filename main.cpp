#include <QApplication>
#include "textbox.h"
#include <QMediaDevices>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TextBox text;
    text.show();

    return app.exec();
}
