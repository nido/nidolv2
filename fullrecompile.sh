#!/bin/bash -x

DEFAULTARGS="--bindir=${HOME}/bin --docdir=${HOME}/Documents --with-lv2dir=${HOME}/.lv2 ${ARGS} --with-fouriersize=35"

# list of extra arguments for configure
while (( "$#" ))
do
ARGS="$ARGS $1"
shift
done

# full cleanup if possible
test -f Makefile && make maintainer-clean
autoreconf -fsi

function build_thingie()
{
	#this is stupid and overbearing but fixes distcheck
	chmod -R 777 "build/$1"
	#"clean" dir before making
	rm -rf "build/$1"
	mkdir -p "build/$1"
	cd "build/$1"
	../../configure ${DEFAULTARGS} $2 || exit
	make || exit
	make check || exit
	make distcheck | exit
	cd -
}

build_thingie default ""
build_thingie sse "--disable-sse3 --with-fourier-size=7"
build_thingie openmp "--enable-openmp --with-fourier-size=7"
build_thingie all "--disable-sse3 --enable-openmp --with-fourier-size=7"
build_thingie maintainer "--enable-maintainer-mode --with-fourier-size=7"
build_thingie msse "--disable-sse3 --enable-maintainer-mode --with-fourier-size=7"
build_thingie mopenmp "--enable-openmp --enable-maintainer-mode --with-fourier-size=7"
build_thingie mall "--disable-sse3 --enable-openmp --enable-maintainer-mode --with-fourier-size=7"
CC=clang build_thingie clang "--enable-sse3 --enable-maintainer-mode --with-fourier-size=7"
