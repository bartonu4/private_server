TEMPLATE = app

QT += qml quick widgets sql

CONFIG += c++11

SOURCES += main.cpp \
    clientsmodel.cpp \
    messagemodel.cpp \
    client.cpp

RESOURCES += qml.qrc
include(../encryption/encryption.pri)
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    client.qml \
    login.qml

HEADERS += \
    myclass.h \
    clientsmodel.h \
    messagemodel.h \
    client.h
