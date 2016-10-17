#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kdc.h"
#include "client.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   // Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_kdcButton_clicked();

    void on_clientButton_clicked();

private:
    Ui::MainWindow *ui;
    KDC *kdc;
    Client * client;
};

#endif // MAINWINDOW_H
