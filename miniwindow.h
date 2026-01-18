#ifndef MINIWINDOW_H
#define MINIWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

class MiniWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MiniWindow(QWidget *parent = nullptr);
    void createMiniWindow(QString text);

private:
    QLabel *minilabel;
    QVBoxLayout *layout;
};

#endif // MINIWINDOW_H
