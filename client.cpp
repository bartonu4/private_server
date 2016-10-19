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

void Client::getInfoFromKDC()
{
    QByteArray buffer = socketToKDC->readAll();
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    if(getStatus() == Client::AUTHORIZATED)
    {
         jsonDocument= QJsonDocument::fromBinaryData(MainServer::aesDecrypt(QByteArray::fromBase64(buffer), getHash()).toUtf8(),QJsonDocument::BypassValidation);
         jsonObject = jsonDocument.object();
    }
    else if(getStatus() == NOT_AUTHORIZATED)
    {
        jsonDocument= QJsonDocument::fromBinaryData(buffer,QJsonDocument::BypassValidation);
        jsonObject = jsonDocument.object();
        if(jsonObject.value("error") != QJsonValue::Undefined)
        {
            qDebug() << "get message from KDC : " << "error=" << jsonObject.value("error").toString();
        }
        else if(jsonObject.value("message") != QJsonValue::Undefined)
        {
            QString message = MainServer::aesDecrypt(QByteArray::fromBase64(jsonObject.value("message").toString().toUtf8()), getHash());
            if(message == "connected")
            {
                setStatus(Client::AUTHORIZATED);
                qDebug() << "Set new status authorizated";
            }
            qDebug() << "get message from KDC : " << "message=" << message;
        }
    }

}

                    Client::STATUS Client::getStatus() const
        {
                    return status;
    }

                    void Client::setStatus(const STATUS &value)
        {
                    status = value;
    }

                    QByteArray Client::getHash() const
        {
                    return hash;
    }

                    void Client::sethash(const QByteArray &value)
        {
                    hash = value;
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
            this->setStatus(Client::NOT_AUTHORIZATED);
      }
      QObject::connect(socketToKDC,&QTcpSocket::readyRead,this,&Client::getInfoFromKDC);
      auto hash         = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
      sethash(hash);
      auto encrypted    = MainServer::aesEncrypt(password,hash);
      QJsonObject jsonObj;
      QJsonDocument document;
      jsonObj["login"] = login;
      jsonObj["message"]= QString(encrypted.toBase64());

      document.setObject(jsonObj);
      socketToKDC->write(document.toBinaryData());
      socketToKDC->flush();

}
