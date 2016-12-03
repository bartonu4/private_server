QT       += core

CONFIG += c++11



HEADERS += $$PWD/encryption.h
SOURCES += $$PWD/encryption.cpp

INCLUDEPATH += ../cryptopp564
release
{
LIBS += -L"../cryptopp564/release" -lcryptopp564
}
debug
{
LIBS += -L"../cryptopp564/debug" -lcryptopp564

}
