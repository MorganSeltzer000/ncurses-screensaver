#!/bin/bash
hello=$(gcc -lncurses -o screensaver program.c 2>&1) ; test -z $hello && ./screensaver || gcc -lncurses -o screensaver program.c
