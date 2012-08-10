#!/usr/bin/env bash

REV=`git log -n1 --pretty=oneline | cut -f1 -d' '`
TYPE=$1

echo $TYPE
cp ./xbmcapp-armeabi-v7a-debug.apk ~/xbmc/dist/xbmcapp-armeabi-v7a-debug-${REV}-${TYPE}.apk
