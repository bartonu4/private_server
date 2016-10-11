#ifndef MAINSERVER_H
#define MAINSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
class MainServer: public QTcpServer
{

public:
    MainServer();
public slots:
    void processConenction();
    void slotNewConnection();


};

#endif // MAINSERVER_H
