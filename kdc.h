#ifndef KDC_H
#define KDC_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMap>
#include <QtSql>
class KDC: public QTcpServer
{
    QMap<quint32, QTcpSocket> usersConnected;
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
