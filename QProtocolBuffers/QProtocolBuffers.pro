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
    $$PWD/modelshelper.h \

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/qobjectmacroplugin.cpp \
    $$PWD/enumgen.cpp \
    $$PWD/objecthelper.cpp \
    $$PWD/propertyhelper.cpp \
    $$PWD/classhelper.cpp \
    $$PWD/modelshelper.cpp \

DEFINES += QT_DEPRECATED_WARNINGS

## Because google protocol buffers
## Do not ship all the needed files for the compiler
## we have to use there source code
#INCLUDEPATH += \
#    $$PWD/../protobuf/src
#DEPENDPATH += \
#    $$PWD/../protobuf/src

linux:!osx:!android {
    LIBS += -L/usr/local/lib/ -lprotoc
    LIBS += -L/usr/local/lib/ -lprotobuf
    PRE_TARGETDEPS += /usr/local/lib/libprotoc.a
    PRE_TARGETDEPS += /usr/local/lib/libprotobuf.a
}

osx{

    LIBS += -L/usr/local/protocol-buffers-apple/platform/x86_64/lib -lprotoc
    PRE_TARGETDEPS += /usr/local/protocol-buffers-apple/platform/x86_64/lib/libprotoc.a

    LIBS += -L/usr/local/protocol-buffers-apple/platform/x86_64/lib -lprotobuf
    PRE_TARGETDEPS += /usr/local/protocol-buffers-apple/platform/x86_64/lib/libprotobuf.a

    INCLUDEPATH += /usr/local/protocol-buffers-apple/platform/x86_64/include
    DEPENDPATH += /usr/local/protocol-buffers-apple/platform/x86_64/include
}

#ios{
#    LIBS += -L/usr/local/protocol-buffers-apple/platform/x86_64/lib/ -lprotoc
#    INCLUDEPATH += /usr/local/protocol-buffers-apple/include
#    DEPENDPATH += /usr/local/protocol-buffers-apple/include
#    PRE_TARGETDEPS += /usr/local/protocol-buffers-apple/platform/x86_64/lib/libprotoc.a
#}

win32{
}





OTHER_FILES += \
    README.md \
    .gitignore \
    .astylerc
