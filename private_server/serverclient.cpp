#include "serverclient.h"

int ServerClient::getId() const
{
    return id;
}

void ServerClient::setId(int value)
{
    id = value;
}

QString ServerClient::getLogin() const
{
    return login;
}

void ServerClient::setLogin(const QString &value)
{
    login = value;
}

ServerClient::STATUS ServerClient::getStatus() const
{
    return status;
}

void ServerClient::setStatus(const STATUS &value)
{
    status = value;
}

unsigned int ServerClient::getAttempts() const
{
    return attempts;
}

void ServerClient::setAttempts()
{
    attempts++;
}

QTcpSocket *ServerClient::getSocket() const
{
    return socket;
}

void ServerClient::setSocket(QTcpSocket *value)
{
    socket = value;
}

QByteArray ServerClient::getHash() const
{
    return hash;
}

void ServerClient::setHash(const QByteArray &value)
{
    hash = value;
}

QHostAddress ServerClient::getAddress()
{
    return socket->peerAddress();
}

quint16 ServerClient::getPort() const
{
    return port;
}

void ServerClient::setPort(const quint16 &value)
{
    port = value;
}

QPair<QString, quint16> ServerClient::getClientToConnect() const
{
    return clientToConnect;
}

void ServerClient::setClientToConnect(const QPair<QString, quint16> &value)
{
    clientToConnect = value;
}

ServerClient::ServerClient()
{
    
}
