#include "client.h"

QString Client::getLogin() const
{
    return login;
}

void Client::setLogin(const QString &value)
{
    login = value;
}

QString Client::getPassword() const
{
    return password;
}

void Client::setPassword(const QString &value)
{
    password = value;
}

void Client::slotNewConnection()
{

}

void Client::establishSecureConnection()
{

}

Client::Client()
{
    //Initialize server
    if(listen(QHostAddress::Any))
    {
        qDebug() << "Listen";
    }

    QObject::connect(this,&QTcpServer::newConnection,this,&Client::slotNewConnection);

    socketToKDC = new QTcpSocket();



}

void Client::connectToKDC()
{
    socketToKDC->setSocketDescriptor(socketDescriptor());
    socketToKDC->connectToHost("localhost", 7777);
    QObject::connect(socketToKDC, &QTcpSocket::connected, this, &Client::establishSecureConnection);
}
