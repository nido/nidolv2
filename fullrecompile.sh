#!/bin/bash -x

DEFAULTARGS="--bindir=${HOME}/bin --docdir=${HOME}/Documents --with-lv2dir=${HOME}/.lv2 ${ARGS} --with-fouriersize=511"

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
	# clean dir before making
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
build_thingie sse "--enable-sse3"
build_thingie openmp "--enable-openmp"
build_thingie all "--enable-sse3 --enable-openmp"
build_thingie maintainer "--enable-maintainer-mode"
build_thingie msse "--enable-sse3 --enable-maintainer-mode"
build_thingie mopenmp "--enable-openmp --enable-maintainer-mode"
build_thingie mall "--enable-sse3 --enable-openmp --enable-maintainer-mode"
