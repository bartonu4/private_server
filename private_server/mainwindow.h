#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kdc.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    KDC *kdc;
    QLineEdit *login;
    QLineEdit *password;

};

#endif // MAINWINDOW_H
