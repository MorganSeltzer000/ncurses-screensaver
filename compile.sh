#!/bin/bash
hello=$(gcc -lncurses -o maybe program.c 2>&1) ; test -z $hello && ./maybe || gcc -lncurses -o maybe program.c
