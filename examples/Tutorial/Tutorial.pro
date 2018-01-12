QT       -= gui
TARGET = Tutorial
TEMPLATE = lib

PROTOS += \
    $$PWD/Example.proto

protobuf_decl.name = protobuf headers
protobuf_decl.input = PROTOS
protobuf_decl.output = ${QMAKE_FILE_BASE}.pb.h

linux:!android:!osx {
        protobuf_decl.commands = /usr/bin/protoc \
            --plugin=protoc-gen-qobject_macro_plugin=$$OUT_PWD/../../QProtocolBuffers/QProtocolBuffers \
            --qobject_macro_plugin_out=$$PWD/  \
            --proto_path=${QMAKE_FILE_IN_PATH} \
            ${QMAKE_FILE_NAME}
}

osx{
    protobuf_decl.commands = /usr/local/protocol-buffers-apple/platform/x86_64/bin/protoc \
            --plugin=protoc-gen-qobject_macro_plugin=$$OUT_PWD/../../QProtocolBuffers/QProtocolBuffers \
            --qobject_macro_plugin_out=$$PWD/  \
            --proto_path=${QMAKE_FILE_IN_PATH} \
            ${QMAKE_FILE_NAME}
}
protobuf_decl.variable_out = HEADERS
QMAKE_EXTRA_COMPILERS += protobuf_decl

protobuf_impl.name = protobuf sources
protobuf_impl.input = PROTOS
protobuf_impl.output = ${QMAKE_FILE_BASE}.pb.cc
protobuf_impl.depends = ${QMAKE_FILE_BASE}.pb.h
protobuf_impl.commands = $$escape_expand(\n)
#protobuf_impl.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += protobuf_impl








PROTOS += \
    $$PWD/Example.proto
    $$PWD/Example.pb.h


