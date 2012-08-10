#!/usr/bin/env bash

# Change dir to git clone
cd ~/xbmc/xbmc.kmn

# Sync upstream/master
echo "Syncing upstream/master"
git checkout master
git pull --rebase upstream master

# Merge changes onto origin/android-tegra2
echo "Merging master onto origin/android-tegra2"
git checkout android-tegra2
git merge origin/master

echo "Done"
