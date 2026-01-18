#include "overlay.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>

using namespace std;
Overlay::Overlay(QWidget *parent) : QWidget(parent) {
    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool
        );
    setAttribute(Qt::WA_TranslucentBackground);
    qDebug() << "works";

    resize(400, 200);
    move(100, 100);

    edit = new QLabel("🤖 AI Assistant Online", this);
    edit->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "background-color: rgba(30, 30, 30, 180);"
        "padding: 12px;"
        "border-radius: 10px;"
        );

    auto *layout = new QVBoxLayout(this);
    updateText("hello");
    qDebug() << "works";
    layout->addWidget(edit);

}

void Overlay::updateText(const QString &text) {
    edit->setText(text);
}
