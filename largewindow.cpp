#include "largewindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
LargeWindow::LargeWindow(QWidget *parent)
    : QWidget(parent) {
    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool
        );
    setAttribute(Qt::WA_TranslucentBackground);

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    setLayout(layout);

    setMaximumWidth(850);
    setMaximumHeight(450);
}

void LargeWindow::createLargeWindow(const QString text) {
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setAttribute(Qt::WA_TranslucentBackground);
    scroll->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollBar { background: transparent; }"
        );
    QWidget *content = new QWidget;
    content->setAttribute(Qt::WA_TranslucentBackground);
    content->setStyleSheet("background: transparent;");

    QVBoxLayout *innerLayout = new QVBoxLayout(content);

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setMaximumWidth(760);
    label->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "background-color: rgba(20, 20, 20, 180);"
        "padding: 12px;"
        "border-radius: 10px;"
        );

    innerLayout->addWidget(label);
    scroll->setWidget(content);
    layout->addWidget(scroll);

    adjustSize();
}
