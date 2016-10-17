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

INCLUDEPATH += ../cryptopp564
release
{
LIBS += -L"../cryptopp564/Win32/Output/Release" -lcryptlib
}
debug
{
LIBS += -L"../cryptopp564/Win32/Output/Debug" -lcryptlib_d

}
#OBJECTS_DIR += "../Botan-1.11.32/build/obj/lib"
#OBJECTS += ../Botan-1.11.32/build/obj/lib/*.obj
FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES +=
