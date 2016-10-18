#include "mainwindow.h"
#include <QApplication>
#include <mainserver.h>
#include <iostream>
#include <memory>
#if defined(WIN32)
#include <windows.h>
#endif

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MainServer::hash("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcda");
    auto res= MainServer::aesEncrypt("nu4",QByteArray::fromHex("C6BB21A8EEC5DF3AA21DB9AE3C9A9EAB"));
    MainServer::aesDecrypt(res,QByteArray::fromHex("C6BB21A8EEC5DF3AA21DB9AE3C9A9EAB"));

    return a.exec();
}
