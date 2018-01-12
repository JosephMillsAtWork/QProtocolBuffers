TEMPLATE = subdirs
CONFIG += ordered

macx{
    # CONFIG += build_apple-protocol-buffers
    build_apple-protocol-buffers{
        system( $$PWD/apple-protocol-buffers.sh )
    }
}

SUBDIRS += \
    $$PWD/QProtocolBuffers/QProtocolBuffers.pro \
    $$PWD/tests/testParser/testParser.pro \
    $$PWD/examples/Tutorial

OTHER_FILES += \
    $$PWD/.qmake.conf \
    $$PWD/apple-protocol-buffers.sh
