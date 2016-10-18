#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H
#include "QTcpSocket"

class ServerClient
{



public:
 enum STATUS{NEW_CONNECTION, IDENTIFIED, KEY_GENERATED, AUTHENTIFICATED, CONNECTED };
    ServerClient();
    int getId() const;
    void setId(int value);
    QString getLogin() const;
    void setLogin(const QString &value);
    STATUS getStatus() const;
    void setStatus(const STATUS &value);
    unsigned int getAttempts() const;
    void setAttempts();
    QTcpSocket *getSocket() const;
    void setSocket(QTcpSocket *value);
    QByteArray getHash() const;
    void setHash(const QByteArray &value);

private:
    int id;
    QString login;
    QByteArray hash;
    STATUS status;
    unsigned int attempts = 0;
    QTcpSocket * socket;
};

#endif // SERVERCLIENT_H
