QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += c++11
}

TARGET   = qthandlebarsjs
TEMPLATE = lib

include(../qthandlebarsjs.pri)

DEFINES += QTHANDLEBARSJS_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}
