#!/bin/sh
# bld.sh

mkdir html >/dev/null 2>&1
# Check to see if semlit is installed
if which semlit.sh >/dev/null 2>&1; then :
  rm -rf html
  mkdir html
  cd html

  if semlit.sh -I .. ../catchup.sldoc; then :
    # We've generated fresh .c and .h files.  Copy them into the project dir.
    for F in *_c.txt; do f=`basename $F _c.txt`;
      rm -f ../$f.c; cp $F ../$f.c; chmod -w ../$f.c
    done
  else :
    echo "semlit.sh failed" >&2
    exit 1
  fi
  cd ..
fi

gcc -g -DSELFTEST -o catchup catchup.c
