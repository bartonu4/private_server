#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpServer>
#include <QTcpSocket>

class Client:public QTcpServer
{
    QString login;
    QString password;
    QTcpSocket *socketToKDC;
public:
    Client();
    void connectToKDC();
    QString getLogin() const;
    void setLogin(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

public slots:
    void slotNewConnection();
    void establishSecureConnection();
};

#endif // CLIENT_H
