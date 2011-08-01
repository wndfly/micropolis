#!/bin/bash

svnsync sync file:///home/liuchong/micropolis-svn/
rm -rf micropolis
git svn clone file:///home/liuchong/micropolis-svn/ --authors-file=/home/liuchong/src/micropoles-user.txt --no-metadata -s micropolis
cd micropolis
if [ "$?" != "0" ]; then
	echo "!!! git svn clone fail !!!\n"
	exit $?;
fi
git remote add origin git@github.com:liu-chong/micropolis.git
git fetch
git merge origin/master
if [ "$?" != "0" ]; then
	echo "!!! git merge origin/master fail !!!";
    exit $?;
fi
git commit -m "sync to git"
git push
