#!/usr/bin/env bash

# Change dir to git clone
cd ~/xbmc/xbmc.kmn

# Sync upstream/master
echo "Syncing upstream/master"
git checkout master
git pull --rebase upstream master
git push

# Merge changes onto origin/android-neon
echo "Merging master onto origin/android-neon"
git checkout android-neon
git merge origin/master

# Merge changes onto origin/android-tegra2
echo "Merging master onto origin/android-tegra2"
git checkout android-tegra2
git merge origin/master

# Reset to android-neon branch
git checkout android-neon

echo "Done"
