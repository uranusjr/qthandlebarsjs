QT       = core

TARGET   = qthandlebarsjs
TEMPLATE = lib

include(../qthandlebarsjs.pri)

DEFINES += QTHANDLEBARSJS_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}
