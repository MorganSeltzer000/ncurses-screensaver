#!/bin/bash
gcc -lncurses -o screensaver program.c
if [ $? == 0 ]
then
	./screensaver
fi
