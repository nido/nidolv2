#!/bin/bash
# full cleanup if possible
test -f Makefile && make maintainer-clean

autoreconf -fsi && ./configure --enable-maintainer-mode --with-fouriersize=512 --bindir=${HOME}/bin --docdir=${HOME}/Documents --with-lv2dir=${HOME}/.lv2 && make check distcheck
