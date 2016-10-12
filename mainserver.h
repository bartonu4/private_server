#ifndef MAINSERVER_H
#define MAINSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
class MainServer: public QTcpServer
{
private:
    static const int CHAR_SIZE = 8;
    static const int MESS_MOD = 512;
    static const int MESS_MOD_RES = 448;
    static const int MESS_LENGHT_SIZE = MESS_MOD - MESS_MOD_RES;
public:
    MainServer();
    static QString hash(QString message);
public slots:
    void processConenction();
    void slotNewConnection();


};

#endif // MAINSERVER_H
