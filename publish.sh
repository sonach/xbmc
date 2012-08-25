#!/usr/bin/env bash

REV=`git log -n1 --pretty=oneline | cut -f1 -d' '`
TYPE=$1
SRC=$2

echo $TYPE
cp ${SRC}/xbmcapp-armeabi-v7a-debug.apk /scratch/xbmc/dist/xbmcapp-armeabi-v7a-debug-${REV}-${TYPE}.apk
