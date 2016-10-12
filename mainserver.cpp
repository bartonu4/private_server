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

}

