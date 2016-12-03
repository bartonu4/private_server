#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "serverclient.h"
#include <QSqlError>
#include "QSqlQuery"
//#include <botan/botan.h>
//#include <botan/symkey.h>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QCryptographicHash>
#include <../encryption/encryption.h>
#include "kdc.h"

//using namespace Botan;

class MessageType
{
public:
    virtual bool request(ServerClient * client) = 0;
};
class MessageIdentify:public MessageType
{
    bool request(ServerClient * client)
        {
            client->setAttempts();
            QJsonObject jsonObject;
            QJsonDocument jsonDocument;
            QByteArray buffer = client->getSocket()->readAll();
            jsonDocument= QJsonDocument::fromBinaryData(buffer,QJsonDocument::BypassValidation);
            jsonObject = jsonDocument.object();
            if(jsonObject["login"] != QJsonValue::Undefined && jsonObject["message"] != QJsonValue::Undefined)
            {
                QString   selectQuery(QString("SELECT password, id, login FROM users WHERE login=:login"));

                qDebug() << selectQuery;
                QSqlQuery query;
                if(!query.prepare(selectQuery))
                {
                    qDebug() << query.lastError();
                }
                query.bindValue(":login",jsonObject["login"].toString());
                if(!query.exec())
                {
                    qDebug() << query.lastError();
                }
                    qDebug() << "query size"<< query.size();
                    query.next();
                    qDebug() << jsonObject["login"] ;

                    if(QString::compare(jsonObject["login"].toString() , query.value(2).toString()) == 0)
                    {


                        QByteArray hash = QCryptographicHash::hash(query.value(0).toByteArray(), QCryptographicHash::Md5);
                        auto messageArray = jsonObject["message"].toArray();
                        qDebug() << "encryptet pass" << messageArray.at(1).toString();
                        QString message;
                        QString port;
                        try
                        {
                            message = MainServer::aesDecrypt(QByteArray::fromBase64(messageArray.at(1).toString().toUtf8()), hash);
                            port =    MainServer::aesDecrypt(QByteArray::fromBase64(messageArray.at(0).toString().toUtf8()), hash);
                        }catch(...)
                        {
                            wrongMessage(client, "Wrong password");
                            return false;
                        }

                        qDebug() << "port " << port.toUShort();
                        if(message == query.value(0).toString())
                        {
                            client->setLogin(jsonObject["login"].toString());
                            client->setId(query.value(1).toInt());
                            client->setStatus(ServerClient::CONNECTED);
                            client->setHash(hash);
                            client->setPort(port.toUShort());
                            return true;
                        }
                        else
                        {
                            wrongMessage(client, "Wrong password");
                        }
                    }


                wrongMessage(client, "Unknown client");


            }
            else {
                wrongMessage(client, "Wrong fields");
            }
            return false;
       }

        void wrongMessage(ServerClient * client, const QString error)
        {
            qDebug() << "error: " << error;
            if(client->getAttempts() < 3)
            {
                QJsonObject jsonAnswer;
                jsonAnswer.insert("error", error);

                client->getSocket()->write(QJsonDocument(jsonAnswer).toBinaryData());
                client->getSocket()->flush();
            }
            else
            {
                client->getSocket()->disconnectFromHost();
            }
        }
};

class MessageConnected: public MessageType
{
//    bool request(ServerClient *client)
//    {
//        QJsonObject jsonObject;
//        QJsonDocument jsonDocument;
//        QByteArray buffer = client->getSocket()->readAll();
//        buffer = QByteArray::fromBase64(MainServer::aesDecrypt(buffer, client->getHash()).toUtf8());
//        jsonDocument= QJsonDocument::fromBinaryData(buffer,QJsonDocument::BypassValidation);
//        jsonObject = jsonDocument.object();
//        if(jsonObject["login"] != QJsonValue::Undefined && jsonObject["port"] != QJsonValue::Undefined)
//        {
//            client->setClientToConnect(jsonObject["login"].toString());
//            return true;
//        }
//        return false;
//    }
};

class MessageHandler
{
    MessageType * message;
public:
    MessageHandler();
    void setStrategy(MessageType &messageType)
    {
        message = &messageType;
    }
    bool execute(ServerClient * client)
    {
        return message->request(client);
    }
};

#endif // MESSAGEHANDLER_H
