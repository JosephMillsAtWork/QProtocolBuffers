QT += core
QT -= gui

CONFIG += \
    c++11

TARGET = QProtocolBuffers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    $$PWD/qobjectmacroplugin.h \
    $$PWD/enumgen.h \
    $$PWD/objecthelper.h \
    $$PWD/propertyhelper.h \
    $$PWD/classhelper.h \
    modelshelper.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/qobjectmacroplugin.cpp \
    $$PWD/enumgen.cpp \
    $$PWD/objecthelper.cpp \
    $$PWD/propertyhelper.cpp \
    $$PWD/classhelper.cpp \
    $$PWD/modelshelper.cpp

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
