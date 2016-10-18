#ifndef MAINSERVER_H
#define MAINSERVER_H
#include <QDebug>
#include <QString>
#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "aes.h"
using CryptoPP::AES;

#include "ccm.h"
using CryptoPP::CBC_Mode;

#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include "assert.h"
class MainServer
{
private:
    static const int CHAR_SIZE = 8;
    static const int MESS_MOD = 512;
    static const int MESS_MOD_RES = 448;
    static const int MESS_LENGHT_SIZE = MESS_MOD - MESS_MOD_RES;
public:
    MainServer();
    static QString hash(QString message);
    static QString aesEncrypt(QString message, QByteArray hash);
    static QString aesDecrypt(QString message, QByteArray hash);



};

#endif // MAINSERVER_H
