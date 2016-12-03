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
    void connectToClient(const QString &name);
public slots:
    void slotNewConnection();
    void slotProcessConnection();
    void establishSecureConnection();
    void getInfoFromKDC();

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
};

#endif // CLIENT_H
