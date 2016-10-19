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
#include <QCryptographicHash>
#include <mainserver.h>


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
                query.bindValue(0,jsonObject["login"].toString());
                if(!query.exec())
                {
                    qDebug() << query.lastError();
                }
                    qDebug() << query.size();
                     query.next();
                if(QString::compare(jsonObject["login"].toString() , query.value(2).toString()) == 0)
                {


                    QByteArray hash = QCryptographicHash::hash(query.value(0).toByteArray(), QCryptographicHash::Md5);
                    QString message = MainServer::aesDecrypt(jsonObject["message"].toVariant().toByteArray(), hash);
                    if(message == query.value(0).toString())
                    {
                        client->setLogin(jsonObject["login"].toString());
                        client->setId(query.value(1).toInt());
                        client->setStatus(ServerClient::AUTHENTIFICATED);
                        client->setHash(hash);
                        return true;
                    }
                    else
                    {
                        wrongMessage(client, "Wrong password");
                    }
                }
                else
                {
                    wrongMessage(client, "Unknown client");
                }
            }
            else {
                wrongMessage(client, "Wrong fields");
            }
            return false;
       }

        void wrongMessage(ServerClient * client, const QString error)
        {
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
