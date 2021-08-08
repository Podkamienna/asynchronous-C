#!/bin/bash

SILNIARES=ressilnia.txt

function testujsilnie () {
  for i in `cat "/tmp/mozilla_alicja0/az406665/test/dane/$1"`; do
	echo $i
	echo $i | "/tmp/mozilla_alicja0/az406665/silnia" >> $SILNIARES
  done
  if diff $SILNIARES "/tmp/mozilla_alicja0/az406665/test/dane/res$1"; then
	  return 1
  else
	  return 0
  fi
}


if [ $# -le 0 ]; then
	echo "Powinien być jeden argument"
	exit 1
fi

if [ -f $SILNIARES ]; then
	rm $SILNIARES
fi

if [ $1 -ge 1 ]; then
	if testujsilnie stud; then
		exit 1
	fi
fi

if [ -f $SILNIARES ]; then
	rm $SILNIARES
fi

if [ $1 -ge 2 ]; then
       if testujsilnie thorough; then
	       exit 1
       fi
fi

if [ -f $SILNIARES ]; then
	rm $SILNIARES
fi
