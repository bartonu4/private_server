#ifndef KDC_H
#define KDC_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QHash>
#include <QtSql>
#include "serverclient.h"
#include "messagehandler.h"
class KDC: public QTcpServer
{
    using STATUS = ServerClient::STATUS;

    QHash<QTcpSocket*, ServerClient*> socketClients;
    //QHash<quint32, QTcpSocket*> usersSocket;
    //QHash<QTcpSocket*, quint32> socketUsers;
    //QHash<QTcpSocket*, STATUS> usersStatus;
    QSqlDatabase db;

    bool getUserFromSqlite(QString login);
public:
    KDC();
    ~KDC();

public slots:
    void processConenction();
    void slotNewConnection();
    void deleteConnection();

};

#endif // KDC_H
