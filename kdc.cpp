#include "kdc.h"

bool KDC::getUserFromSqlite(QString login)
{
    QSqlQuery query("SELECT login FROM users");
    while (query.next())
    {
             QString user = query.value(0).toString();

             qDebug() << user;

            if(! QString::compare(user,login))
            {
                return true;
            }
    }
    return false;

}

KDC::KDC(): QTcpServer()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName(":users.sqlite");
    qDebug() <<QCoreApplication::applicationDirPath();
    db.setDatabaseName("../private-server/users.sqlite");
    if(db.open())
    {
        qDebug() << "Db opened";
    }
    else
    {
        qDebug() << "Last error" << db.lastError().text();
    }
    qDebug() << getUserFromSqlite("root");

    //Initialize server
    if(listen(QHostAddress::Any,7777))
    {
        qDebug() << "Listen";
    }

    QObject::connect(this,&QTcpServer::newConnection,this,&KDC::slotNewConnection);
}

KDC::~KDC()
{

}
void KDC::processConenction()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    ServerClient *client = socketClients.value(socket);
    switch (client->getStatus()) {
    case STATUS::NEW_CONNECTION:
    {
        MessageHandler handler;
        MessageIdentify mtype;
        handler.setStrategy(mtype);
        if(handler.execute(socketClients.value(socket)))
        {
            auto keys = socketClients.keys();
            for(int i = 0; i < keys.size(); i++)
            {
                QJsonObject jsonObject;
                QJsonDocument jsonDocument;
                jsonObject.insert("action", "userConnected");
                jsonObject.insert("login", client->getLogin());
                jsonDocument.setObject(jsonObject);
                keys[i]->write(MainServer::aesEncrypt(jsonDocument.toBinaryData(), socketClients.value(keys[i])->getHash()));
                keys[i]->flush();
                jsonObject["login"] = socketClients.value(keys[i])->getLogin();
                jsonDocument.fromJson(jsonObject);
                socket->write(MainServer::aesEncrypt(jsonDocument.toBinaryData(), client->getHash()));
                socket->flush();
            }
        }
        break;
    }
    default:
        break;
    }

}

void KDC::slotNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    ServerClient * client= new ServerClient;
    client->setSocket(socket);
    client->setStatus(STATUS::NEW_CONNECTION);
    socketClients.insert(socket, client);


    QObject::connect(socket,&QTcpSocket::readyRead,this,&KDC::processConenction);
    QObject::connect(socket,&QTcpSocket::disconnected, this, &KDC::deleteConnection);
     qDebug() << "New connection established to "<< socket->peerAddress().toString();


}

void KDC::deleteConnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    ServerClient *client  = socketClients.value(socket);
    socketClients.remove(socket);
    socket->deleteLater();

    delete client;
    qDebug() << "client deleted";
}
