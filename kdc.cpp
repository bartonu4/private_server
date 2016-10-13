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
    db.setDatabaseName("users.sqlite");
    if(db.open())
    {
        qDebug() << "Db opened";
    }
    else
    {
        qDebug() << "Last error" << db.lastError().text();
    }
    qDebug() << getUserFromSqlite("root");
    if(listen(QHostAddress::Any,7777))
    {
        qDebug() << "Listen";
    }

    QObject::connect(this,newConnection,this,slotNewConnection);
}

KDC::~KDC()
{

}
void KDC::processConenction()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray buffer = socket->readAll();
    socket->write(QByteArray(QString("nu4"+socket->peerAddress().toString()+socket->peerPort()+"local port %1").arg(serverPort()).toUtf8()));
    socket->flush();

        qDebug() << buffer;
}

void KDC::slotNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    QObject::connect(socket,&QTcpSocket::readyRead,this,processConenction);
     qDebug() << "New connection established to "<< socket->peerAddress().toString();


}
