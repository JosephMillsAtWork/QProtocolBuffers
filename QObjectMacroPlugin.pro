QT += core
QT -= gui

CONFIG += c++11

TARGET = QObjectMacroPlugin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    $$PWD/qobjectmacroplugin.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/qobjectmacroplugin.cpp

DEFINES += QT_DEPRECATED_WARNINGS








unix:!macx: LIBS += -L/usr/local/lib/ -lprotoc
unix:!macx: LIBS += -L/usr/local/lib/ -lprotobuf
unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libprotoc.a
unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libprotobuf.a

## Because google protocol buffers
## Do not ship all the needed files for the compiler
## we have to use there source code
INCLUDEPATH += \
    /usr/local/include \
    $$PWD/protobuf/src
DEPENDPATH += \
    /usr/local/include \
    $$PWD/protobuf/src

OTHER_FILES += \
    README.md \
    .gitignore \
    .astylerc
