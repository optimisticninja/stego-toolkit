#!/bin/sh

if [ "$#" -ne 1 ] || ! [ -d "$1" ]; then
	echo "Usage: $0 DIRECTORY" >&2
	exit 1
else
	docker run --rm -it -v $1:/puzzles --entrypoint /bin/bash stego-toolkit:latest
fi

