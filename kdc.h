#ifndef KDC_H
#define KDC_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QHash>
#include <QtSql>

class KDC: public QTcpServer
{
    enum STATUS{NEW_CONNECTION, IDENTIFIED, KEY_GENERATED, AUTHENTIFICATED, CONNECTED };
    QHash<quint32, QTcpSocket*> usersSocket;
    QHash<QTcpSocket*, quint32> socketUsers;
    QHash<quint32, STATUS> usersStatus;
    QSqlDatabase db;

    bool getUserFromSqlite(QString login);
public:
    KDC();
    ~KDC();

public slots:
    void processConenction();
    void slotNewConnection();

};

#endif // KDC_H
