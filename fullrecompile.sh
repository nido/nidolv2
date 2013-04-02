#!/bin/bash
# full cleanup if possible
test -f Makefile && make maintainer-clean

autoreconf -si && ./configure --with-fouriersize=512 --bindir=${home}/bin --with-lv2dir=${HOME}/.lv2 && make check
