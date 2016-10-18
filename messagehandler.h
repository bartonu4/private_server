#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "serverclient.h"
#include "QSqlQuery"
//#include <botan/botan.h>
//#include <botan/symkey.h>
#include <memory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
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
            jsonDocument.fromBinaryData(buffer);
            jsonObject = jsonDocument.object();
            if(jsonObject['login'] != QJsonValue::Undefined && jsonObject['message'] != QJsonValue::Undefined)
            {
                QSqlQuery query(QString("SELECT password, id FROM users WHERE login='%1'").arg(jsonObject['login'].toString()));
                if(query.size() > 0)
                {
                    query.next();
                    QByteArray hash = QCryptographicHash::hash(query.value(0).toByteArray(), QCryptographicHash::Md5);
                    QString message = MainServer::aesDecrypt(jsonObject['message'].toString(), hash);
                    if(message == query.value(0).toString())
                    {
                        client->setLogin(jsonObject['login'].toString());
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
