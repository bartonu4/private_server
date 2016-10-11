#include "mainserver.h"

MainServer::MainServer():QTcpServer()
{
    if(listen(QHostAddress::Any,7777))
    {
        qDebug() << "Listen";
    }

    QObject::connect(this,newConnection,this,slotNewConnection);


}

void MainServer::processConenction()
{

}

void MainServer::slotNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    QObject::connect(this,newConnection,this,processConenction);
     qDebug() << "New connection established to "<< socket->peerAddress().toString();



}
