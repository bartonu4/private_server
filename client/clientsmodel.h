#ifndef CLIENTSMODEL_H
#define CLIENTSMODEL_H
#include <QObject>
#include <QStringList>

class ClientsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList data READ data WRITE add NOTIFY dataChanged)
    QStringList m_data;


public:
    ClientsModel(QObject *parent = 0);

    Q_INVOKABLE void add(const QStringList &client);
    QStringList data() ;


signals:
    void dataChanged();
};




#endif // CLIENTSMODEL_H
