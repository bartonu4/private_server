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
    MainServer::aesEncrypt("nu4");

    return a.exec();
}
