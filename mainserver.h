#ifndef MAINSERVER_H
#define MAINSERVER_H
#include <QDebug>
#include <QString>
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



};

#endif // MAINSERVER_H
