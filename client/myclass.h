#ifndef MYCLASS_H
#define MYCLASS_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>
#include <QQmlContext>
#include <QColor>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <clientsmodel.h>
#include <client.h>
class MyClass : public QObject
{
    Q_OBJECT
    Client client;
public:
    MyClass(){
        QObject::connect(&client, &Client::connectionStatus,
                         this, &waitForStatusClient );
        QObject::connect(&client, &Client::newClientAvailable,
                         this, &addClients );
        QObject::connect(&client, &Client::getNewMessage,
                         this, &processNewMessage );
        QObject::connect(&client, &Client::deleteClient,
                         this, &deleteClient );
    }
    QQmlApplicationEngine *engine;
public slots:
    void  submit(const QString &msg) {

        qDebug() << "Called the C++ slot with message:" << msg;
        QStringList credentials = msg.split(",");
        qDebug() << credentials;
        client.setLogin(credentials.at(0));
        client.setPassword(credentials.at(1));
        client.connectToKDC();


        //        QObject *qmlRoot = engine->rootObjects().first();
        //        QQuickItem *item = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("item"));

        //        auto clientsModel = item->findChild<QObject*>("clientsModel");
        //        QMetaObject::invokeMethod(clientsModel, "add", Q_ARG(QStringList, QStringList("nu4")));


    }
    void chooseClient(const QString &name)
    {
        client.connectToClient(name);
    }
    void sendMessage(const QString &recepient, const QString &message)
    {
        qDebug() << "sendMessage" << recepient << message;
        client.sendMessage(recepient, message);
    }
    void waitForStatusClient()
    {
        qDebug() << "status" << client.getStatus();
        QObject *qmlRoot = engine->rootObjects().first();
        auto loader = qmlRoot->findChild<QQuickItem*>("loader");
        QQuickItem *loaderItem = qvariant_cast<QQuickItem*> (loader->property("item"));
        //QQuickItem *loaderSource = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("source"));

        auto dialog = loaderItem->findChild<QObject*>("messageDialogObj");
        qDebug() << loader << loaderItem << dialog;
        switch (client.getStatus())
        {
        case Client::STATUS::NO_CONNECTION:
            dialog->setProperty("text", "Can not connect to Server");
            dialog->setProperty("visible", true);
            break;
        case Client::STATUS::NOT_AUTHORIZATED:
            dialog->setProperty("text", "Wrong login or password");
            dialog->setProperty("visible", true);
            break;
        case Client::STATUS::AUTHORIZATED:
            //Load objects synchronious
            QMetaObject::invokeMethod(loader, "setClient");

            QQuickItem *loaderItem = qvariant_cast<QQuickItem*> (loader->property("item"));
            auto clientModel = loaderItem->findChild<QObject*>("listClients");
            QObject::connect(clientModel, SIGNAL(chooseClient(QString)), this, SLOT(chooseClient(QString)));

            auto messageModel = loaderItem->findChild<QObject*>("messageModel");
            QMetaObject::invokeMethod(messageModel, "setMyName", Q_ARG(QString, client.getLogin()));
            QObject::connect(messageModel, SIGNAL(sendMessage(QString, QString)), this, SLOT(sendMessage(QString,QString)));
            qDebug() << " HERERERER";
            break;
        }


    }
    void addClients(QString newClient){

        QObject *qmlRoot = engine->rootObjects().first();
        QQuickItem *item = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("item"));

        auto clientsModel = item->findChild<QObject*>("clientsModel");
        QMetaObject::invokeMethod(clientsModel, "add", Q_ARG(QStringList, QStringList(newClient)));

    }
    void deleteClient(QString client){

        QObject *qmlRoot = engine->rootObjects().first();
        QQuickItem *item = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("item"));

        auto clientsModel = item->findChild<QObject*>("clientsModel");
        QMetaObject::invokeMethod(clientsModel, "deleteClient", Q_ARG(QString, client));

    }
    void processNewMessage(QString sender, QString message)
    {
        QObject *qmlRoot = engine->rootObjects().first();
        auto loader = qmlRoot->findChild<QQuickItem*>("loader");
        QQuickItem *loaderItem = qvariant_cast<QQuickItem*> (loader->property("item"));
        auto messageModel = loaderItem->findChild<QObject*>("messageModel");
        qDebug() << "sender" << sender << message;
        QMetaObject::invokeMethod(messageModel, "add", Q_ARG(QString, sender), Q_ARG(QString, QString("0")), Q_ARG(QString, message));
    }
};
#endif // MYCLASS_H
