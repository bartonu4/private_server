#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <../encryption/encryption.h>

#define INPUT "wdvP52W8SVh0lb1JkXifpSAPUxEtAXKHzi9btc+SO8QxD332srqpLXfjj2un0SkwGSbdv3pvhS099kX1kMxfJZV4PUrgjIdDbwNh68zhjsgcgG3l4YlUrm6iwGnjyTjt30QX7bKAdVehuWII92ANWlnCqnsXATwL6eb+Vt0aaNBBmgrIFQxDwW5T2X1Bi10/lA1R5vgX+rEDxUTzBriUmUyG9i6j8qu3+dYxxye/8lQ="
#define HASH_LENGTH 128
#define CHAR_SIZE 8
QByteArray initHash()
{
    QByteArray hash ="";
    for(int i = 0; i < HASH_LENGTH / CHAR_SIZE; ++i)
    {
        hash.append((char)0);
    }
    return hash;
}
void printBin(QByteArray hash)
{
    QString printStr = "";
    for(int i = 0; i < hash.size() * CHAR_SIZE; ++i)
    {
        if((hash.at(i / CHAR_SIZE) >> (7 - (i % 8))) & 1)
        {
            printStr.append("1");
        }
        else {
            printStr.append("0");
        }
    }
    qDebug() << "Key(bin): " << printStr;
}

void printHex(QByteArray hash)
{
    qDebug() << "Key(hex): " << hash.toHex();
}

int main(int argc, char *argv[])
{
    QByteArray input = QByteArray::fromBase64(INPUT);
    QByteArray hash = initHash();
    int lastElem = hash.size() - 1;
    QTime start = QTime::currentTime();
    qDebug() << "Started: ";
    while(((unsigned char)hash.at(12)) != 0x1f)
    {
        int index = lastElem;
        bool go = true;
        while(go)
        {
            hash[index] = (char)(((unsigned char)hash.at(index)) + 1);
            if(hash[index] == 0)
            {
                if(!index)
                {
                    break;
                }
                --index;
                continue;
            }
            go = false;
        }
        QByteArray result = QByteArray::fromBase64(MainServer::aesDecrypt(input, hash).toUtf8());
        if(result.contains("message"))
        {
            break;
        }

        if(!index)
        {
            break;
        }

    }

    QTime finish = QTime::currentTime();
    qDebug() << start.msecsTo(finish);
    QJsonDocument document = QJsonDocument::fromBinaryData(MainServer::postDecryption(MainServer::aesDecrypt( input, hash)),QJsonDocument::BypassValidation);
    QJsonObject object = document.object();
    qDebug() << "Action: " << object["action"].toString();
    qDebug() << "With value: " << object["message"].toString();
    qDebug() << "To " << object["login"].toString();
    printBin(hash);
    printHex(hash);



    return 0;
}
