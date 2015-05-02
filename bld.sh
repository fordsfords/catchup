#!/bin/sh
# bld.sh

# Check to see if SEMLIT is installed.
rm -rf html
mkdir html
cd html
if semlit.sh -I .. ../catchup.sldoc; then :
  cp *.c ..
else :
  echo "semlit.sh failed" >&2
  exit 1
fi
cd ..

gcc -g -DSELFTEST -o catchup catchup.c
