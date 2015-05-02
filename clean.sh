#!/bin/sh
# clean.sh

./tidy.sh

# only clean up the semi-literate doc of this machine has SEMLIT installed
if which semlit.sh >/dev/null 2>&1; then :
	rm -rf html
fi
rm -f catchup_*.tar *.c *.h
