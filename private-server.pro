#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T20:08:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = private-server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        user.cpp \
        kdc.cpp \
        mainserver.cpp \
    client.cpp

HEADERS  += mainwindow.h\
             user.h \
            kdc.h \
            mainserver.h \
    client.h

FORMS    += mainwindow.ui
