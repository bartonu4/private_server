
#include <clientsmodel.h>


ClientsModel::ClientsModel(QObject *parent):
    QObject(parent)
{

   // m_data << "orange" << "skyblue";
}

void ClientsModel::add(const QStringList &client)
{

    m_data.append(client);
    emit dataChanged();
}

void ClientsModel::deleteClient(const QString &client)
{
    m_data.removeOne(client);
    emit dataChanged();
}

QStringList ClientsModel::data()
{
    return m_data;
}

