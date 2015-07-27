QT       += testlib
QT       -= gui

TARGET    = qthandlebarsjstest
CONFIG   += c++11 console
CONFIG   -= app_bundle

TEMPLATE  = app

include(../qthandlebarsjs.pri)

DEFINES += SRCDIR=\\\"$$PWD/../src\\\"

INCLUDEPATH += $$PWD/../src

SOURCES += \
    test_main.cpp \
    homepagetest.cpp

HEADERS += \
    homepagetest.h
