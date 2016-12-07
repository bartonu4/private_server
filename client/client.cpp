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
    qDebug() << "asbhfkwhjevgfbkwejbvfkwejb";
    QTcpSocket *socket = nextPendingConnection();
    //socket->waitForReadyRead();

    QObject::connect(socket,&QTcpSocket::readyRead,this,&Client::slotProcessConnection);
    QObject::connect(socket,&QTcpSocket::disconnected, this, &Client::deleteConnection);


}

void Client::slotProcessConnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    auto name = connectedToClients.key(socket);
    QByteArray sessHash;
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;
    auto buffer = socket->readAll();

    if(connectedToClients.contains(name))
    {
        sessHash = clientsHash.value(name);
        jsonDocument = QJsonDocument::fromBinaryData(MainServer::postDecryption(MainServer::aesDecrypt( buffer, sessHash)),QJsonDocument::BypassValidation);
    }
    else
    {
        sessHash = hash;
        jsonDocument = QJsonDocument::fromBinaryData(MainServer::postDecryption(MainServer::aesDecrypt(QByteArray::fromBase64( buffer), sessHash)),QJsonDocument::BypassValidation);

    }




    qDebug() << "buffer" << buffer;

    jsonObject = jsonDocument.object();
    if(jsonObject["action"].toString() == "CtC")
    {
        auto name = jsonObject["login"].toString();
        qDebug() << "slotNewConnection " << name;
        connectedToClients.insert(name, socket);
        clientsHash.insert(name, QByteArray::fromBase64(jsonObject["key"].toString().toUtf8()));
    }
    else if(jsonObject["action"].toString() == "message")
    {
        qDebug() << "GET NEW MESSAGE";
        emit getNewMessage(jsonObject["login"].toString(), jsonObject["message"].toString());
    }


}

void Client::deleteConnection()
{
    qDebug() << "connection deleted";
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
        else if(jsonObject["action"].toString()  == "CtC")
        {


            QTcpSocket *socket = new QTcpSocket;
            socket->bind();
            connectedToClients.insert(jsonObject["login"].toString(), socket);
            qDebug() << jsonObject["port"].toInt();

            socket->connectToHost(jsonObject["address"].toString(), jsonObject["port"].toInt() );
            if(socket->waitForConnected(10000))
            {
                socket->write(jsonObject["message"].toString().toUtf8());
                socket->flush();
                qDebug() << "message" << jsonObject["message"].toString().toUtf8();
                clientsHash.insert(jsonObject["login"].toString(), QByteArray::fromBase64(jsonObject["key"].toString().toUtf8()));

                QObject::connect(socket,&QTcpSocket::readyRead,this,&Client::slotProcessConnection);
                QObject::connect(socket,&QTcpSocket::disconnected, this, &Client::deleteConnection);
            }



            qDebug() << "CtC" << jsonObject["login"].toString() << jsonObject["port"].toString().toUShort();
        }
        else if (jsonObject["action"].toString() == "userDeleted")
        {
            auto clientName = jsonObject["login"].toString();
            connectedToClients.remove(clientName);
            clientsHash.remove(clientName);
            emit deleteClient(clientName);
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
        emit connectionStatus();
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

void Client::sendMessage(const QString &recepient, const QString &message)
{


    qDebug() << " in client SEND MESS";
    auto keys = connectedToClients.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        if(keys[i] == recepient)
        {
            qDebug() << "find i = " << i;
            QJsonDocument document;
            QJsonObject jsonObj;
            jsonObj["action"] = "message";
            jsonObj["login"] = recepient;
            jsonObj["message"] = message;
            document.setObject(jsonObj);
            auto socket = connectedToClients.value(keys[i]);
            auto sessionHash = clientsHash.value(recepient);
            qDebug() <<"HASH" <<sessionHash;
            qDebug() << "MSG: " << QString(MainServer::aesEncrypt(MainServer::preEncryption(document), sessionHash).toBase64());
            socket->write(MainServer::aesEncrypt(MainServer::preEncryption(document), sessionHash));
            socket->flush();
        }

    }
}

void Client::connectToClient(const QString &name)
{

    if(connectedToClients.contains(name))
        return;

    QJsonObject jsonObj;
    QJsonDocument document;

    jsonObj["action"] = "CtC";
    jsonObj["login"] = name;

    document.setObject(jsonObj);

    socketToKDC->write(MainServer::aesEncrypt(MainServer::preEncryption(document), getHash()));
    socketToKDC->flush();

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

        QByteArray hash          = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
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
