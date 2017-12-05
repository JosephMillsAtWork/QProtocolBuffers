#!/bin/bash
#        --plugin=protoc-gen-qobject_macro_plugin=$(pwd)/QObjectMacroPlugin \
/usr/local/bin/protoc \
        --plugin=protoc-gen-qprotocol_buffers_plugin=$(pwd)/QProtocolBuffers \
        --qobject_macro_plugin_out=$(pwd)/qobj \
	--proto_path=$(pwd) \
	$(pwd)/BushingEvent.proto 



