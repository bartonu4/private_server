#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kdc.h"
#include "client.h"
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
    void on_clientButton_clicked();

    void on_kdcButton_clicked();
    void submitClick();
private:
    Ui::MainWindow *ui;
    KDC *kdc;
    Client *client;
    QLineEdit *login;
    QLineEdit *password;

};

#endif // MAINWINDOW_H
