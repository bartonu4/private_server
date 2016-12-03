#include "kdc.h"

QHash<QTcpSocket*, ServerClient*> KDC::socketClients;
bool KDC::getUserFromSqlite(QString login)
{
    QSqlQuery query("SELECT login FROM users");
    while (query.next())
    {
        QString user = query.value(0).toString();

        qDebug() << user;

        if(! QString::compare(user,login))
        {
            return true;
        }
    }
    return false;

}

KDC::KDC(): QTcpServer()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName(":users.sqlite");
    qDebug() <<QCoreApplication::applicationDirPath();
    db.setDatabaseName("../private_server/users.sqlite");
    if(db.open())
    {
        qDebug() << "Db opened";
    }
    else
    {
        qDebug() << "Last error" << db.lastError().text();
    }
    qDebug() << getUserFromSqlite("root");

    //Initialize server
    if(listen(QHostAddress::Any,7777))
    {
        qDebug() << "Listen";
    }

    QObject::connect(this,&QTcpServer::newConnection,this,&KDC::slotNewConnection);
}

KDC::~KDC()
{

}
void KDC::processConenction()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    ServerClient *client = socketClients.value(socket);
    switch (client->getStatus()) {
    case STATUS::NEW_CONNECTION:
    {
        MessageHandler handler;
        MessageIdentify mtype;
        handler.setStrategy(mtype);
        if(handler.execute(client))
        {

            QJsonObject jsonSuccessObject;
            QJsonDocument jsonSuccessDocument;
            jsonSuccessObject.insert("message", QString(MainServer::aesEncrypt("connected", client->getHash()).toBase64()));
            jsonSuccessDocument.setObject(jsonSuccessObject);
            socket->write(jsonSuccessDocument.toBinaryData());
            socket->flush();
            auto keys = socketClients.keys();
            for(int i = 0; i < keys.size(); i++)
            {
                if(keys[i] != client->getSocket())
                {
                    QJsonObject jsonObject;
                    QJsonDocument jsonDocument;
                    jsonObject.insert("action", "userConnected");
                    jsonObject.insert("login", client->getLogin());
                    jsonDocument.setObject(jsonObject);
                    qDebug() << jsonDocument.toBinaryData();
                    keys[i]->write(MainServer::aesEncrypt(jsonDocument.toBinaryData().toBase64(), socketClients.value(keys[i])->getHash()));
                    keys[i]->flush();
                    jsonObject["login"] = socketClients.value(keys[i])->getLogin();
                    jsonDocument.setObject(jsonObject);
                    socket->write(MainServer::aesEncrypt(jsonDocument.toBinaryData().toBase64(), client->getHash()));
                    socket->flush();
                    qDebug() << "success";
                }
            }

        }

        break;
    }
    case STATUS::CONNECTED:
    {
        qDebug() << "pizda";
        QJsonObject jsonFromObject;
        QJsonDocument jsonFromDocument;

        QByteArray buffer = client->getSocket()->readAll();
        buffer = MainServer::postDecryption(MainServer::aesDecrypt(buffer, client->getHash()));
        qDebug() << "DUFFER: " << buffer;
        jsonFromDocument= QJsonDocument::fromBinaryData(buffer,QJsonDocument::BypassValidation);
        jsonFromObject = jsonFromDocument.object();
        qDebug() << jsonFromObject["action"].toString();
        if (jsonFromObject["action"] == "CtC")
        {

            QString login;
            if(jsonFromObject["login"] != QJsonValue::Undefined)
            {
               login = jsonFromObject["login"].toString();
               //port = client->setClientToConnect(jsonFromObject["port"].toString().toUShort());

            }
            qDebug() << "KDC get key: " << login;

            QJsonObject jsonObject;
            QJsonDocument jsonDocument;
            jsonObject.insert("action", "CtC");
            jsonObject["login"] = client->getLogin();
            jsonObject["key"] = QString(MainServer::generateKey());

            auto keys = socketClients.keys();
            for(int i = 0; i < keys.size(); i++)
            {
                if(keys[i] != client->getSocket())
                {
                    if(QString::compare(socketClients.value(keys[i])->getLogin(),login) == 0)
                    {
                        auto clientTo = socketClients.value(keys[i]);
                        //jsonObject["clientTo"] = clientTo->getLogin();
                        jsonDocument.setObject(jsonObject);
                        QByteArray buffer = MainServer::aesEncrypt(MainServer::preEncryption(jsonDocument), clientTo->getHash());

                        QJsonObject jsonFirstClientObject;
                        jsonFirstClientObject.insert("action", "CtC");
                        jsonFirstClientObject["message"] = QString(buffer);
                        jsonFirstClientObject["login"]   = clientTo->getLogin();
                        jsonFirstClientObject["port"]   = clientTo->getPort();
                        jsonFirstClientObject["key"]   = jsonObject["key"];
                        jsonDocument.setObject(jsonFirstClientObject);
                        socket->write(MainServer::aesEncrypt(MainServer::preEncryption(jsonDocument), client->getHash()));
                        socket->flush();



                    }
                }
            }

        }


    }
    default:
        break;
    }

}

void KDC::slotNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    ServerClient * client= new ServerClient;
    client->setSocket(socket);
    client->setStatus(STATUS::NEW_CONNECTION);
    socketClients.insert(socket, client);


    QObject::connect(socket,&QTcpSocket::readyRead,this,&KDC::processConenction);
    QObject::connect(socket,&QTcpSocket::disconnected, this, &KDC::deleteConnection);
    qDebug() << "New connection established to "<< socket->peerAddress().toString();


}

void KDC::deleteConnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    ServerClient *client  = socketClients.value(socket);
    socketClients.remove(socket);
    socket->deleteLater();

    delete client;
    qDebug() << "client deleted";
}
