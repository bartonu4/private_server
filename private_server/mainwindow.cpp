#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = new QLineEdit();
    password = new QLineEdit();
    kdc = new KDC{};
    ui->textBrowser->setText(QString("KDC listenig on port %1").arg(kdc->serverPort()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete kdc;
}



