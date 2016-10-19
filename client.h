#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>
#include "mainserver.h"
class Client:public QTcpServer
{
    enum STATUS{AUTHORIZATED, NOT_AUTHORIZATED};
    STATUS status;
    QString login;
    QString password;
    QByteArray hash;
    QTcpSocket *socketToKDC;
public:
    Client();
    void connectToKDC();
    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    STATUS getStatus() const;
    void setStatus(const STATUS &value);

    QByteArray getHash() const;
    void sethash(const QByteArray &value);

public slots:
    void slotNewConnection();
    void establishSecureConnection();
    void getInfoFromKDC();
};

#endif // CLIENT_H
