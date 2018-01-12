#!/bin/bash
set -x

echo Building Google Protobuf for Mac OS X / iOS.
echo Use 'tail -f build.log' to monitor progress.
## branch
VERSION=3.5.x
## install prefix
PREFIX=/usr/local/protocol-buffers-apple/


## osx settings
DARWIN_RELEASE=$(uname -r)
CORES=$(sysctl hw.ncpu | awk '{print $2}')

if [[ ! -d ${PREFIX} ]];
then
    mkdir -p ${PREFIX}
    mkdir -p ${PREFIX}/platform
fi


OSX_SDK=$(xcodebuild -showsdks \
    | grep macosx | sort | tail -n 1 | awk '{print substr($NF, 7)}'
    )

IOS_SDK=$(xcodebuild -showsdks \
    | grep iphoneos | sort | tail -n 1 | awk '{print substr($NF, 9)}'
    )

XCODEDIR=`xcode-select --print-path`

MACOSX_PLATFORM=${XCODEDIR}/Platforms/MacOSX.platform
#MACOSX_SYSROOT=${MACOSX_PLATFORM}/Developer/MacOSX${OSX_SDK}.sdk

IPHONEOS_PLATFORM=${XCODEDIR}/Platforms/iPhoneOS.platform
IPHONEOS_SYSROOT=${IPHONEOS_PLATFORM}/Developer/SDKs/iPhoneOS${IOS_SDK}.sdk

IPHONESIMULATOR_PLATFORM=${XCODEDIR}/Platforms/iPhoneSimulator.platform
IPHONESIMULATOR_SYSROOT=${IPHONESIMULATOR_PLATFORM}/Developer/SDKs/iPhoneSimulator${IOS_SDK}.sdk

CC=clang
CFLAGS="-DNDEBUG -g -O0 -pipe -fPIC -fcxx-exceptions"
CXX=clang
CXXFLAGS="${CFLAGS} -std=c++11 -stdlib=libc++"
LDFLAGS="-stdlib=libc++"
LIBS="-lc++ -lc++abi"

####################################
# Cleanup any earlier build attempts
####################################


echo "------------------ Cleanup --------------------"

cd /tmp


if [ -d ${PREFIX} ];
then
    rm -rf ${PREFIX}
fi

mkdir ${PREFIX}
mkdir ${PREFIX}/platform


##########################################
# Fetch Google Protobuf from source.
##########################################

echo "------------------ Fetch protobuf --------------------"
cd /tmp

if [[ -d /tmp/protobuf-${VERSION} ]];
then
    rm -rf /tmp/protobuf-${VERSION} ;
fi

git clone https://github.com/google/protobuf.git protobuf-${VERSION}
cd protobuf-${VERSION}

if [[ ${VERSION} != "master" ]];
then
    git checkout ${VERSION}
fi

sh autogen.sh


###############################################################
# Replace "namespace google" with "namespace google_public"
# in all source/header files. This is to address a
# namespace collision issue when building for recent
# versions of iOS. Apple is using the protobuf library
# internally, and embeds it as a private framework.
###############################################################


echo "------------------ Fixup namespace --------------------"
cd /tmp/protobuf-${VERSION}/src/google/protobuf
sed -i '' -e 's/namespace\ google /namespace\ google_public /g' $(find . -name \*.h -type f)
sed -i '' -e 's/namespace\ google /namespace\ google_public /g' $(find . -name \*.cc -type f)
sed -i '' -e 's/namespace\ google /namespace\ google_public /g' $(find . -name \*.proto -type f)

sed -i '' -e 's/google::protobuf/google_public::protobuf/g' $(find . -name \*.h -type f)
sed -i '' -e 's/google::protobuf/google_public::protobuf/g' $(find . -name \*.cc -type f)
sed -i '' -e 's/google::protobuf/google_public::protobuf/g' $(find . -name \*.proto -type f)


#####################
# x86_64 for Mac OS X
#####################
echo "------------------ Mac OS X --------------------"
cd /tmp/protobuf-${VERSION}

## this is the 1st time this runs...  make sure that there is a makefile before clean
if [[ -f /tmp/protobuf-${VERSION}/Makefile ]];
then
    make clean
fi

./configure --disable-shared --enable-static \
    --prefix=${PREFIX}/platform/x86_64 \
    --exec-prefix=${PREFIX}/platform/x86_64 \
    "CC=${CC}" \
    "CFLAGS=${CFLAGS} -arch x86_64" \
    "CXX=${CXX}" \
    "CXXFLAGS=${CXXFLAGS} -arch x86_64" \
    "LDFLAGS=${LDFLAGS}" \
    "LIBS=${LIBS}"

make -j ${CORES}
make install


###########################
# i386 for iPhone Simulator
###########################
#echo "------------------ iPhone Simulator --------------------"
#cd /tmp/protobuf-${VERSION}
#make clean
#./configure --build=x86_64-apple-darwin${DARWIN_RELEASE} \
#    --host=i386-apple-darwin${DARWIN_RELEASE} \
#    --with-protoc=${PREFIX}/platform/x86_64/bin/protoc \
#    --disable-shared --enable-static \
#    --prefix=${PREFIX}/platform/i386 \
#    --exec-prefix=${PREFIX}/platform/i386 \
#    "CC=${CC}"  \
#    "CFLAGS=${CFLAGS} -miphoneos-version-min=${IOS_SDK} -arch i386 -isysroot ${IPHONESIMULATOR_SYSROOT}" \
#    "CXX=${CXX}" \
#    "CXXFLAGS=${CXXFLAGS} -arch i386 -isysroot ${IPHONESIMULATOR_SYSROOT}" \
#    "LDFLAGS=-arch i386 -miphoneos-version-min=${IOS_SDK} ${LDFLAGS}" \
#    "LIBS=${LIBS}"
#make -j ${CORES}
#make install

##################
# armv7 for iPhone
##################
echo "------------------ armv7 iPhone --------------------"
cd /tmp/protobuf-${VERSION}
make clean
./configure --build=x86_64-apple-darwin${DARWIN_RELEASE} \
    --host=armv7-apple-darwin${DARWIN_RELEASE} \
    --with-protoc=${PREFIX}/platform/x86_64/bin/protoc \
    --disable-shared --enable-static \
    --prefix=${PREFIX}/platform/armv7 \
    --exec-prefix=${PREFIX}/platform/armv7
    "CC=${CC}" \
    "CFLAGS=${CFLAGS} -miphoneos-version-min=${IOS_SDK} -arch armv7 -isysroot ${IPHONEOS_SYSROOT}" \
    "CXX=${CXX}" \
    "CXXFLAGS=${CXXFLAGS} -arch armv7 -isysroot ${IPHONEOS_SYSROOT}" \
    LDFLAGS="-arch armv7 -miphoneos-version-min=${IOS_SDK} ${LDFLAGS}" \
    "LIBS=${LIBS}"

make -j ${CORES}
make install


###################
# armv7s for iPhone
###################


echo "------------------ armv7s iPhone --------------------"
cd /tmp/protobuf-${VERSION}
make clean
./configure --build=x86_64-apple-darwin${DARWIN_RELEASE} \
    --host=armv7s-apple-darwin${DARWIN_RELEASE} \
    --with-protoc=${PREFIX}/platform/x86_64/bin/protoc \
    --disable-shared --enable-static \
    --prefix=${PREFIX}/platform/armv7s \
    --exec-prefix=${PREFIX}/platform/armv7s \
    "CC=${CC}" \
    "CFLAGS=${CFLAGS} -miphoneos-version-min=${IOS_SDK} -arch armv7s -isysroot ${IPHONEOS_SYSROOT}" \
    "CXX=${CXX}" \
    "CXXFLAGS=${CXXFLAGS} -arch armv7s -isysroot ${IPHONEOS_SYSROOT}" \
    LDFLAGS="-arch armv7s -miphoneos-version-min=${IOS_SDK} ${LDFLAGS}" \
    "LIBS=${LIBS}"

make -j ${CORES}
make install

##################
# arm64 for iPhone
##################


echo "------------------ arm64 iPhone --------------------"
cd /tmp/protobuf-${VERSION}
make clean
./configure --build=x86_64-apple-darwin${DARWIN_RELEASE} \
    --host=arm --with-protoc=${PREFIX}/platform/x86_64/bin/protoc \
    --disable-shared --enable-static \
    --prefix=${PREFIX}/platform/arm64 \
    --exec-prefix=${PREFIX}/platform/arm64 \
    "CC=${CC}" \
    "CFLAGS=${CFLAGS} -miphoneos-version-min=${IOS_SDK} -arch arm64 -isysroot ${IPHONEOS_SYSROOT}" \
    "CXX=${CXX}" \
    "CXXFLAGS=${CXXFLAGS} -arch arm64 -isysroot ${IPHONEOS_SYSROOT}" \
    LDFLAGS="-arch arm64 -miphoneos-version-min=${IOS_SDK} ${LDFLAGS}" \
    "LIBS=${LIBS}"

make -j ${CORES}
make install

############################
# Create Universal Libraries
############################
echo "---Running Lippo ---"
cd ${PREFIX}/platform
mkdir universal
lipo x86_64/lib/libprotobuf.a \
    arm64/lib/libprotobuf.a \
    armv7s/lib/libprotobuf.a \
    armv7/lib/libprotobuf.a \
    -create -output universal/libprotobuf.a

lipo x86_64/lib/libprotobuf-lite.a \
    arm64/lib/libprotobuf-lite.a \
    armv7s/lib/libprotobuf-lite.a \
    armv7/lib/libprotobuf-lite.a \
    -create -output universal/libprotobuf-lite.a

lipo x86_64/lib/libprotoc.a \
    arm64/lib/libprotoc.a \
    armv7s/lib/libprotoc.a \
    armv7/lib/libprotoc.a \
    -create -output universal/libprotobuf.a




echo "-- Extra Packaging  --"



