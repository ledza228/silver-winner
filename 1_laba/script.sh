#!/bin/bash

if [ $# -lt 3 ]
then
	echo "usage: $0 minFileSize maxFileSize Path" >&2
	exit
fi

find $3 -type f -size -$2b -size +$1b -printf "%k\t" -exec readlink -f {} \; | sort -h
