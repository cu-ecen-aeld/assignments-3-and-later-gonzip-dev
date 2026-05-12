#!/bin/sh


if [ ! $# -eq 2 ];then
	echo "Incorrect number of arguments"
	exit 1
else

	if [ ! -r "$1"  ];then
		echo "Argument 1 is not a directory file"
		exit 1
	fi
fi

numfiles="$(find $1 -type f | wc -l)"
numwords="$(grep -r $2 $1 | wc -l)"
echo "The number of files are $numfiles and the number of matching lines are $numwords"





