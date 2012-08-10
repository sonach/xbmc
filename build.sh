#!/bin/bash -x
#/usr/bin/env bash

BUILD_DIR=$1
TOOLS_PREFIX=$2

echo $BUILD_DIR
echo $TOOLS_PREFIX

function build {
	TYPE=$1
    BRANCH=$2
	TOOLS_PREFIX=$3

	echo $TOOLS_PREFIX

    git clean -f -d -x
    git checkout $BRANCH
    echo "    tools/android/depends"
    cd tools/android/depends
    echo "        Run bootstrap"
    ./bootstrap || exit 1
    echo "        Run configure"
    ./configure \
        --with-ndk=${TOOLS_PREFIX}/android-ndk-r7-crystax-5.beta2 \
        --with-sdk=${TOOLS_PREFIX}/android-sdk-linux \
        --with-toolchain=${TOOLS_PREFIX}/android-sdk-linux/android-9 \
        --with-tarballs=${TOOLS_PREFIX}/tarballs \
        --with-sdk-platform=android-9 || exit 1
    echo "        Building depends"
    make -j15 || exit 1
    echo "        Ensuring depends were built 100%"
    make || exit 1
    echo "    Building XBMC"
    cd ../../../
    make -C tools/android/depends/xbmc || exit 1
    echo "    Ensuring XBMC was built 100%"
    make || exit 1
    echo "    Making apk explicitly just in case"
    make apk || exit 1
    echo "    Publishing"
    ~/xbmc/xbmc.kmn/publish.sh $TYPE || exit 1
}

# Clone repo
git clone ~/xbmc/xbmc.kmn $BUILD_DIR
cd $BUILD_DIR

# Build NEON
echo "Building NEON"
build neon-script-testing android-neon $TOOLS_PREFIX

# Build non-NEON
echo "Building non-NEON"
build non-neon-script-testing android-tegra2 ${TOOLS_PREFIX}/xbmc.tegra2

# Remind user to cleanup build dir
echo "Don't forget to cleanup / remove ${BUILD_DIR}"

