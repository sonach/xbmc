#!/bin/bash -x
#/usr/bin/env bash

BUILD_DIR=$1
TOOLS_PREFIX=$2
TYPE=$3

echo $BUILD_DIR
echo $TOOLS_PREFIX
echo $TYPE

cd $BUILD_DIR

bash build_deps.sh

bash build_xbmc.sh

bash ~/xbmc/xbmc.kmn/publish.sh $TYPE || exit 1
