#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHash>
#include <QCryptographicHash>
#include "../encryption/encryption.h"
class Client: public QTcpServer
{
    Q_OBJECT


public:

    enum STATUS{AUTHORIZATED, NO_CONNECTION, NOT_AUTHORIZATED};

    Client();
    void connectToKDC();
    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QByteArray getHash() const;
    void sethash(const QByteArray &value);

    STATUS getStatus() const;
    void setStatus(const STATUS &value);

    void sendMessage(const QString &recepient, const QString &message);

public slots:
    void slotNewConnection();
    void slotProcessConnection();
    void deleteConnection();
    void establishSecureConnection();
    void getInfoFromKDC();
    void connectToClient(const QString &name);

private:
    STATUS status;
    QString login;
    QString password;
    QByteArray hash;
    QTcpSocket *socketToKDC;
    QHash<QString, QTcpSocket*> connectedToClients;
    QHash<QString, QByteArray> clientsHash;
    QList<QString> availableClients;
signals:
    void connectionStatus();
    void newClientAvailable(QString client);
    void getNewMessage(QString client, QString message);
    void deleteClient(QString client);
};

#endif // CLIENT_H
