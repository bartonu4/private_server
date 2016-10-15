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

ServerClient::ServerClient()
{

}
