#!/bin/sh
gcc -c -fPIC plugin.c -o plugin.o
gcc plugin.o -shared -o plugin.so
