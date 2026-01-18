#pragma once
#include <QWidget>
#include <QLabel>
#include <QString>

class Overlay : public QWidget {
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = nullptr);
    void updateText(const QString &text);

private:
    QLabel *edit;
};
