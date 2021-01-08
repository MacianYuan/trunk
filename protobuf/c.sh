#!/bin/sh
export LD_LIBRARY_PATH=/opt/protobuf-2.5.0/src/.libs:$LD_LIBRARY_PATH
/opt/protobuf-2.5.0/src/.libs/protoc  message.proto --cpp_out=.
/opt/protobuf-2.5.0/src/.libs/protoc  message.proto --java_out=.
mv com/slark/pengder/protocol/MessageProtos.java .
rm -fr com
