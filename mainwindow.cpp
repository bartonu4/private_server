#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete kdc;
}



void MainWindow::on_kdcButton_clicked()
{
    kdc = new KDC;
}

void MainWindow::on_clientButton_clicked()
{
    client = new Client;
}
