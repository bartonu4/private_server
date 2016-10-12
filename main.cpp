#include "mainwindow.h"
#include <QApplication>
#include <mainserver.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MainServer::hash("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcda");

    return a.exec();
}
