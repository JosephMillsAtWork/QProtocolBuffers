
TARGET = tst_testparser
CONFIG   += console
CONFIG   -= app_bundle
QT       += testlib
QT       -= gui
TEMPLATE = app

INCLUDEPATH += \
    $$PWD/../../QProtocolBuffers \
    $$PWD/../../QProtocolBuffers/protobuf/src
DEPENDPATH += \
    $$PWD/../../QProtocolBuffers \
    $$PWD/../../QProtocolBuffers/protobuf/src


HEADERS += \
    $$PWD/../../QProtocolBuffers/objecthelper.h

SOURCES += \
    $$PWD/../../QProtocolBuffers/objecthelper.cpp \
    $$PWD/tst_testparser.cpp


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRCDIR=\\\"$$PWD/\\\"
