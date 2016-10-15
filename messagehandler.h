#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "serverclient.h"
#include "QSqlQuery"
class MessageType
{
public:
    virtual void request(ServerClient * client) = 0;
};
class MessageIdentify:public MessageType
{
    void request(ServerClient * client)
    {
        QByteArray buffer = client->getSocket()->readAll();

        QSqlQuery query("SELECT login FROM users");
        while (query.next())
        {
                 QString user = query.value(0).toString();

                 qDebug() << user;

                if(! QString::compare(user,QString(buffer)))
                {
                    return ;
                    qDebug() << "true";
                }
        }
        return ;
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
    void execute(ServerClient * client)
    {
        message->request(client);
    }
};

#endif // MESSAGEHANDLER_H
