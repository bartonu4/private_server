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

void Client::slotProcessConnection()
{

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    auto buffer = socket->readAll();
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    jsonDocument = QJsonDocument::fromBinaryData(QByteArray::fromBase64(MainServer::aesDecrypt(buffer, getHash()).toUtf8()),QJsonDocument::BypassValidation);
    jsonObject = jsonDocument.object();
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
        jsonDocument = QJsonDocument::fromBinaryData(QByteArray::fromBase64(MainServer::aesDecrypt(buffer, getHash()).toUtf8()),QJsonDocument::BypassValidation);
        jsonObject = jsonDocument.object();
        qDebug() << jsonObject;
        if(jsonObject["action"].toString()  == "userConnected")
        {
            QString newClientLogin = jsonObject["login"].toString();
            availableClients.append(newClientLogin);

            qDebug() << "new client" << newClientLogin;

            emit newClientAvailable(newClientLogin);
        }
        else if(jsonObject["action"].toString()  == "client to connect")
        {
            auto keys = connectedToClients.keys();
            for(int i = 0; i < keys.size(); i++)
            {
                if(keys[i] == jsonObject["login"].toString() )
                {
                   auto socket = connectedToClients.value(keys[i]);
                   socket->connectToHost("127.0.0.1",jsonObject["port"].toString().toUShort() );
                   socket->write(jsonObject["message"].toString().toUtf8());

                }
            }
             qDebug() << "client to connect" << jsonObject["login"].toString();
            clientsHash.insert(jsonObject["login"].toString(), jsonObject["hash"].toString().toUtf8());
        }
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
    emit connectionStatus();

}

Client::STATUS Client::getStatus() const
{
    return status;
}

void Client::setStatus(const STATUS &value)
{
    status = value;
}

void Client::connectToClient(const QString &name)
{
    QTcpSocket *socket  = new QTcpSocket();
    socketToKDC->bind();
    connectedToClients.insert(name, socket);


    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj["action"] = "client to connect";
    jsonObj["login"] = name;
    jsonObj["port"] = socket->localPort();
    document.setObject(jsonObj);
    socketToKDC->write(MainServer::aesEncrypt(document.toBinaryData().toBase64(), getHash()));
    socketToKDC->flush();
    QObject::connect(socket,&QTcpSocket::readyRead,this,&Client::slotProcessConnection);

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
    QObject::connect(socketToKDC,&QTcpSocket::readyRead,this,&Client::getInfoFromKDC);
    QObject::connect(this,&QTcpServer::newConnection,this,&Client::slotNewConnection);



}

void Client::connectToKDC()
{

    socketToKDC->connectToHost("127.0.0.1", 7777);

    //QObject::connect(socketToKDC, &QTcpSocket::connected, this, &Client::establishSecureConnection);

    if (socketToKDC->waitForConnected(1000))
    {
        qDebug("Connected!");
        this->setStatus(Client::NOT_AUTHORIZATED);

        auto hash          = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
        sethash(hash);
        auto encrypted     = MainServer::aesEncrypt(password,hash);
        auto portEncrypted = MainServer::aesEncrypt(QString("%1").arg(serverPort()), hash);
        QJsonObject jsonObj;
        QJsonDocument document;
        qDebug() << QString(encrypted.toBase64());
        qDebug() << "port" << serverPort();

        QJsonArray array = {QString(portEncrypted.toBase64()), QString(encrypted.toBase64())};
        jsonObj["login"] = login;
        jsonObj["message"]= array;

        document.setObject(jsonObj);
        socketToKDC->write(document.toBinaryData());
        socketToKDC->flush();
        return;
    }
    this->setStatus(Client::NO_CONNECTION);
    emit connectionStatus();

}
