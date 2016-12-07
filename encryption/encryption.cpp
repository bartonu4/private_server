#include "encryption.h"

MainServer::MainServer()
{



}

QString MainServer::hash(QString message)
{
    unsigned long long lenght = message.length() * CHAR_SIZE;
    int toAdd = (((lenght % MESS_MOD) > MESS_MOD_RES) ? (MESS_MOD - ((lenght % MESS_MOD) % MESS_MOD_RES)) : (MESS_MOD_RES - (lenght % MESS_MOD)));
    for(int i = 0; i < toAdd / CHAR_SIZE; i++)
    {
        message.append(" ");
    }
    int mess = MESS_LENGHT_SIZE;
    for(int i = 1; i <= MESS_LENGHT_SIZE;)
    {
        char tmp = 0;
        for(int j = 1; j <= CHAR_SIZE; j++, i++)
        {
            int a = sizeof(lenght);
            tmp = tmp | (((lenght >> (MESS_LENGHT_SIZE - i)) & 1) << (CHAR_SIZE - j));
        }
        message.append(tmp);
    }
    qDebug() << "lenght " << lenght << " toAdd " << toAdd << " lenght " <<  message.length() * CHAR_SIZE << " message " << message;
    return "hash";
}

QByteArray MainServer::aesEncrypt(QString message, QByteArray hash)
{

         qDebug() <<AES::DEFAULT_KEYLENGTH << hash.size() <<AES::BLOCKSIZE ;
       // prng.GenerateBlock(key, sizeof(key));

        //byte iv[AES::BLOCKSIZE];
        //prng.GenerateBlock(iv, sizeof(iv));

        string plain = "CBC Mode Test";
        string cipher, encoded, recovered;

        /*********************************\
        \*********************************/

        // Pretty print key
        encoded.clear();
        StringSource((unsigned char*)hash.data(), hash.size(), true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource

        qDebug() << "key: " << QString::fromStdString(encoded) ;

        // Pretty print iv
        encoded.clear();
        StringSource((unsigned char*)hash.data(), hash.size(), true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource
        qDebug() << "iv: " << QString::fromStdString(encoded)  ;

        /*********************************\
        \*********************************/
        try
        {
            qDebug() << "plain text: " << QString::fromStdString(plain)  ;

            CBC_Mode< AES >::Encryption e;
            e.SetKeyWithIV((unsigned char*)hash.data(), hash.size(),(unsigned char*) hash.data());

            // The StreamTransformationFilter removes
            //  padding as required.
            StringSource s(message.toStdString(), true,
                new StreamTransformationFilter(e,
                    new StringSink(cipher)
                ) // StreamTransformationFilter
            ); // StringSource

        }
        catch(const CryptoPP::Exception& e)
        {


        }
;

        encoded.clear();
        StringSource(cipher, true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource


        qDebug() << "encrypted text: " << QString::fromStdString(encoded)  ;
        return QByteArray::fromStdString(cipher);



}

QString MainServer::aesDecrypt(QByteArray message, QByteArray hash)
{

    /*********************************\
    \*********************************/

        // Pretty print
        string encoded;
        string recovered;
        StringSource((unsigned char*)hash.data(), hash.size(), true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource
         qDebug() << "key: " << QString::fromStdString(encoded) ;
        encoded.clear();

        StringSource(message.toStdString(), true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource
        qDebug() << "cipher text: " << QString::fromStdString(encoded)  ;

        /*********************************\
        \*********************************/

        try
        {
            CBC_Mode< AES >::Decryption d;
            d.SetKeyWithIV((unsigned char *)hash.data(), hash.size(), (unsigned char *)hash.data());

            // The StreamTransformationFilter removes
            //  padding as required.
            StringSource s(message.toStdString(), true,

                new StreamTransformationFilter(d,
                    new StringSink(recovered)
                ) // StreamTransformationFilter

            ); // StringSource



            qDebug() << "recovered text: " << QString::fromStdString(recovered)  ;
        }
        catch(const CryptoPP::Exception& e)
        {
            qDebug() <<QString::fromStdString(e.GetWhat());

        }
        return QString::fromStdString(recovered);
}

QByteArray MainServer::preDecryption(const QString &string)
{
    return QByteArray::fromBase64(string.toUtf8());
}

QByteArray MainServer::postDecryption(const QString &string)
{
    return QByteArray::fromBase64(string.toUtf8());
}

QByteArray MainServer::generateKey()
{
    AutoSeededRandomPool rnd;

    // Generate a random key
    SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock( key, key.size() );
    QByteArray bkey;
    foreach (auto byte, key) {
        bkey.append((char) byte);
//        bkey.append((char)0);
    }
//    for(int i = 13; i < bkey.size(); i++)
//    {
//        bkey[i] = (char) qrand();
//    }

    return bkey;

}

QByteArray MainServer::preEncryption(const QJsonDocument &document)
{
    return document.toBinaryData().toBase64();
}

