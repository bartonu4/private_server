#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = nullptr;
    login = new QLineEdit();
    password = new QLineEdit();
     kdc = new KDC{};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete kdc;
}



void MainWindow::on_clientButton_clicked()
{
    if(client == nullptr)
    {
       client = new Client;
    }

    QWidget *widget = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout;

    QPushButton *submit =new QPushButton("sbumit");
    QObject::connect(submit, &QPushButton::clicked, this, &MainWindow::submitClick);

    password->setEchoMode(QLineEdit::Password);
    vbox->addWidget(login);
    vbox->addWidget(password);
    vbox->addWidget(submit);

    widget->setLayout(vbox);
    widget->show();
    setCentralWidget(widget);

}

void MainWindow::on_kdcButton_clicked()
{

}

void MainWindow::submitClick()
{
    QWidget *widget = new QWidget(this);
    client->setLogin(login->text());
    client->setPassword(password->text());
    qDebug() << "here" << client->getLogin() << client->getPassword();
    client->connectToKDC();

    widget->show();
    setCentralWidget(widget);

}
