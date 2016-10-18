#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T20:08:52
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = private-server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        user.cpp \
        kdc.cpp \
        mainserver.cpp \
    client.cpp \
    serverclient.cpp \
    messagehandler.cpp

HEADERS  += mainwindow.h\
             user.h \
            kdc.h \
            mainserver.h \
    client.h \
    serverclient.h \
    messagehandler.h

INCLUDEPATH += ../../cryptopp564
release
{
LIBS += -L"../private_server/cryptopp564/release" -lcryptopp564
}
debug
{
LIBS += -L"../private_server/cryptopp564/debug" -lcryptopp564

}

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES +=
