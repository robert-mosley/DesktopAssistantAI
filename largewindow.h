#ifndef LARGEWINDOW_H
#define LARGEWINDOW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>

class LargeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LargeWindow(QWidget *parent = nullptr);
    void createLargeWindow(QString text);

private:
    QLabel *largelabel;
    QVBoxLayout *layout;
};

#endif // MINIWINDOW_H
