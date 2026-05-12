#!/bin/sh


if [ ! $# -eq 2 ] ; then
	echo "Incorrect number of arguments"
	exit 1
fi

DIR=$(dirname $1)

if [ ! -d $DIR ] ; then
	mkdir -p $DIR
fi

if (! echo $2 > $1) ; then
	echo "File coud not be created"
	exit 1
fi








