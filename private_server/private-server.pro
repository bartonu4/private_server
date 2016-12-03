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
include(../encryption/encryption.pri)

SOURCES += main.cpp\
        mainwindow.cpp\
        user.cpp \
        kdc.cpp \
    serverclient.cpp \
    messagehandler.cpp \


HEADERS  += mainwindow.h\
             user.h \
            kdc.h \
    serverclient.h \
    messagehandler.h



FORMS    += mainwindow.ui


RESOURCES +=
