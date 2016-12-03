#include "messagemodel.h"

QString MessageModel::recipient() const
{
    return m_recipient;
}

void MessageModel::setRecipient(const QString &recipient)
{
    if (recipient == m_recipient)
        return;

    m_recipient = recipient;

    const QString filterString = QString::fromLatin1(
                "(recipient = '%1' AND sender = 'me') OR (recipient = 'me' AND sender='%1')").arg(m_recipient);
    setFilter(filterString);
    select();
    qDebug() << "recipient changed " << recipient;
    emit recipientChanged();
}
static QString conversationsTableName = "Conversations";

MessageModel::MessageModel(QObject *parent) :  QSqlTableModel(parent)
{

    if (QSqlDatabase::database().tables().contains(conversationsTableName)) {
        // The table already exists; we don't need to do anything.
        qDebug() << "Exis";

    }
    else {

        QSqlQuery query;
        if (!query.exec(
                    "CREATE TABLE IF NOT EXISTS 'Conversations' ("
                    "'sender' TEXT NOT NULL,"
                    "'recipient' TEXT NOT NULL,"
                    "'message' TEXT NOT NULL)"))
        {
            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
        }
    }
    setTable(conversationsTableName);
    //setSort(0, Qt::AscendingOrder);
    // Ensures that the model is sorted correctly after submitting a new row.
    setEditStrategy(QSqlTableModel::OnManualSubmit);




}



QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(rowCount() - index.row() - 1);
    //qDebug() << role<< sqlRecord.value(role - Qt::UserRole -1 );
    return sqlRecord.value(role - Qt::UserRole-1);
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles = QSqlTableModel::roleNames();
    roles[roleSender] = "sender";
    roles[roleRecipient] = "recipient";
    roles[roleMessage] = "message";
    return roles;
}

void MessageModel::add( const QString & _recepient, const QString & _message)
{

    QSqlRecord newRecord = record();
    newRecord.setValue(roleSender - Qt::UserRole -1, "me");
    newRecord.setValue(roleRecipient - Qt::UserRole -1, _recepient);
    newRecord.setValue(roleMessage - Qt::UserRole -1, _message);
    if (!insertRecord(rowCount(), newRecord)) {
        qWarning() << "Failed to send message:" << lastError().text()<<rowCount();
        return;
    }

    if(!submitAll())
    {
        qDebug() << "Failed to submit" << lastError().text();
    }
}


