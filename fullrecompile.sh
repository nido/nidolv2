#!/bin/bash
# full cleanup if possible
test -f Makefile && make maintainer-clean

# create directory needed
test -d m4 || mkdir m4

autoreconf -fsi && ./configure && make
