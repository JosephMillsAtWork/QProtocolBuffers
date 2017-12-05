#!/bin/bash
/usr/local/bin/protoc \
        --plugin=protoc-gen-qprotocol_buffers_plugin=$(pwd)/QProtocolBuffers \
        --qobjectbuffers_plugin_out=$(pwd)/qobj \
	--proto_path=$(pwd) \
	$(pwd)/Example.proto


