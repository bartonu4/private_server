#include "mainserver.h"

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

QString MainServer::aesEncrypt(QString message)
{
    CryptoPP::AutoSeededRandomPool prng;

        byte key[AES::DEFAULT_KEYLENGTH];
        prng.GenerateBlock(key, sizeof(key));

        byte iv[AES::BLOCKSIZE];
        prng.GenerateBlock(iv, sizeof(iv));

        string plain = "CBC Mode Test";
        string cipher, encoded, recovered;

        /*********************************\
        \*********************************/

        // Pretty print key
        encoded.clear();
        StringSource(key, sizeof(key), true,
            new HexEncoder(
                new StringSink(encoded)
            ) // HexEncoder
        ); // StringSource

        qDebug() << "key: " << QString::fromStdString(encoded) ;

        // Pretty print iv
        encoded.clear();
        StringSource(iv, sizeof(iv), true,
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
            e.SetKeyWithIV(key, sizeof(key), iv);

            // The StreamTransformationFilter removes
            //  padding as required.
            StringSource s(plain, true,
                new StreamTransformationFilter(e,
                    new StringSink(cipher)
                ) // StreamTransformationFilter
            ); // StringSource

        }
        catch(const CryptoPP::Exception& e)
        {

            exit(1);
        }

        /*********************************\
        \*********************************/

        // Pretty print
        encoded.clear();
        StringSource(cipher, true,
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
            d.SetKeyWithIV(key, sizeof(key), iv);

            // The StreamTransformationFilter removes
            //  padding as required.
            StringSource s(cipher, true,
                new StreamTransformationFilter(d,
                    new StringSink(recovered)
                ) // StreamTransformationFilter
            ); // StringSource



            qDebug() << "recovered text: " << QString::fromStdString(recovered)  ;
        }
        catch(const CryptoPP::Exception& e)
        {

            exit(1);
        }
}

