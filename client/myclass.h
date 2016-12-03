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
            QMetaObject::invokeMethod(loader, "setClient");

            break;
        }


    }
    void addClients(QString newClient){

        //        qDebug() << "add clients";
        //        QObject *qmlRoot = engine->rootObjects().first();
        //        QQuickItem *item = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("item"));

                QObject *qmlRoot = engine->rootObjects().first();
                QQuickItem *item = qvariant_cast<QQuickItem*> (qmlRoot->findChild<QQuickItem*>("loader")->property("item"));

                auto clientsModel = item->findChild<QObject*>("clientsModel");
                QMetaObject::invokeMethod(clientsModel, "add", Q_ARG(QStringList, QStringList(newClient)));

        //qDebug() << qmlRoot->findChild<QObject*>("loader")->property("item");
        //        QMetaObject::invokeMethod(, "setModel",
        //                Q_ARG(QVariant, map));

        //        QStringList data;
        //        data << "nasdfsdfu4" << "sdf";
        //        qDebug() <<  item->findChild<QObject*>("clientsModel")->setProperty("data", data);
    }
};
#endif // MYCLASS_H
