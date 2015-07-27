defineReplace(mode) {
    CONFIG(release, debug|release):return(release)
    else:return(debug)
}

QT += qml

DESTDIR = ../bin/$$mode()

BUILD_DIR = ../build/$$TARGET/$$mode()
PRECOMPILED_DIR = $$BUILD_DIR
OBJECTS_DIR = $$BUILD_DIR
MOC_DIR = $$BUILD_DIR
RCC_DIR = $$BUILD_DIR
UI_DIR = $$BUILD_DIR


# Add your files here

HEADERS += \
    $$PWD/src/qthandlebarsjs_global.h \
    $$PWD/src/qthandlebarsjs.h \
    $$PWD/src/handlebars.h \
    $$PWD/src/helpermanager.h

SOURCES += \
    $$PWD/src/handlebars.cpp \
    $$PWD/src/helpermanager.cpp

RESOURCES += \
    $$PWD/src/qthandlebarsjs.qrc
