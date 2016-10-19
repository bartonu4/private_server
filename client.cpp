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
    qDebug() << "connected";
}

Client::Client()
{
    //Initialize server
    if(listen(QHostAddress::Any))
    {
        qDebug() << "Listen";
    }

    //QObject::connect(this,&QTcpServer::newConnection,this,&Client::slotNewConnection);

    socketToKDC = new QTcpSocket();
    socketToKDC->bind();



}

void Client::connectToKDC()
{

    socketToKDC->connectToHost("127.0.0.1", 7777);

    //QObject::connect(socketToKDC, &QTcpSocket::connected, this, &Client::establishSecureConnection);

      if (socketToKDC->waitForConnected(1000))
      {
            qDebug("Connected!");
      }

      auto hash         = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
      auto encrypted    = MainServer::aesEncrypt(hash,hash);
      QJsonObject jsonObj;
      QJsonDocument document;
      jsonObj["login"] = login;
      jsonObj["message"]= QString::fromStdString(encrypted.toStdString());

      document.setObject(jsonObj);
      socketToKDC->write(document.toBinaryData());
      socketToKDC->flush();






}
