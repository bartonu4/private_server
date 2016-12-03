#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H
#include <QMap>
#include <QSql>
#include <QtSql/QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



class MessageModel: public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:

    enum Roles {
        roleSender = Qt::UserRole + 1,
        roleRecipient,
        roleMessage
    };
    QString recipient() const;
    void setRecipient(const QString &recipient);

    MessageModel(QObject *parent = 0);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void add(const QString & _recepient, const QString & _message);
signals:
    void recipientChanged();

private:

    QString m_recipient;

};

#endif // MESSAGEMODEL_H
