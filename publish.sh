#!/usr/bin/env bash

REV=`git log -n1 --pretty=oneline | cut -f1 -d' '`
DATE=`date +%Y-%m-%d`
TYPE=$1
SRC=$2

echo $TYPE
echo $REV
cp ${SRC}/xbmcapp-armeabi-v7a-debug.apk /scratch/xbmc/dist/xbmcapp-armeabi-v7a-debug-${DATE}-${TYPE}-${REV}.apk
