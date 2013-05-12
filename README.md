Nido's lv2 filters                                           {#mainpage}
==================

to make ./configure, run: mkdir m4; autoreconf -si
mkdir is needed because libtool is set up to put stuff there and thus configure.ac expects the directory to exists

maintainer requirements:
autoconf-archive
libtool
lv2peg

nidoamp
-------------------------

implemented [here](@ref nidoamp.c)
WIP name. Spectral filter making use of fourier analysis to alter the
signal. Current version alters the signal in the frequency space and
transforms it back.

next version will use a convolution kernel based on the filter
parameters as applied on the signal.

Input to the fourier transformation is a block of pcm samples. the block
size is dependent on [`FOURIER_SIZE`](@ref FOURIER_SIZE). The default value is 512.

the complex kernel is 1 for each point in the input fourier transform
output that is unaffected, 0 for each frequency block cancelled out,
newer versions may use other values as well. An inverse fourier
transformation of the complex kernel will result in the convolution
kernel which starts at the beginning of this block.

The kernel corresponding to the next block of data may be derived in the
same fashion as the current kernel. the difference of these kernels
defines the kernel to be applied in the convolution of the input signal.

So, in order to compute the second point in the signal the convolution
kernel would be `1 / FOURIER_SIZE * mykernel + (FOURIER_SIZE - 1) /
FOURIER_SIZE * nextkernel`, and one needs the next kernel.

This requires a buffer of `2 * FOURIER_SIZE` of input to be full in order
to compute the next point.


To compute point X (naive):

	output[x] = convolve(input[x], kernel[x])
	kernel[x] = computed_kernel[x - x % FOURIER_SIZE]
	computed_kernel[x] = idft(fourier_matrix[x])
	fourier_matrix[x] = get_matrix(complex_buffer)
	complex_buffer = dft(input[x])

OpenBSD
-------
Requirements on openbsd:
autoconf
automake
AC_MSG_ERROR working

Notes
-----

To those who really care about memory usage:

This plugin makes use of fftw3 in order to do fourier transformations.
In order to truly reclaim all memory used, at the end of it all i should
run fftwf\_cleanup() to make sure everything of fftw is properly cleaned up.
However, as it turns out, running that function crashes practically every
DAW i tested. I assume DAW's somehow found an use for fftw3, so cleaning up
their plans from under their noses seems like a bad idea.

So if you want to be sure you reclaim everything and use my plugin, be sure
to run fftw\_cleanup() afterwards

Known Problems
--------------
Trying to compile it with pcc on Fedora18 fails. Reason why may have to
do with an error in Fedora 18, but the cause has not been properly analysed.
For now, if you want to compile using pcc, be prepared to get the following
output.

	make[1]: Entering directory `/home/nido/code/nidolv2'
	/bin/sh ./libtool --tag=CC   --mode=link /usr/bin/pcc -Wall -g  -module -avoid-version -shared   -o nidoamp.la -rpath /home/nido/code/nidolv2/dest/lib/lv2/nidoamp.lv2 nidoamp.lo ringbuffer.lo innerproduct.lo cpuid.lo  -lfftw3f -lfftw3f -lfftw3f -lfftw3f -lm 
	libtool: link: /usr/bin/pcc -shared  -fPIC -DPIC  .libs/nidoamp.o .libs/ringbuffer.o .libs/innerproduct.o .libs/cpuid.o   -lfftw3f -lm    -Wl,-soname -Wl,nidoamp.so -o .libs/nidoamp.so
	/usr/bin/ld: /usr/lib64/pcc/x86_64-unknown-linux-gnu/1.1.0.DEVEL/lib/crtbegin.o: relocation R_X86_64_32S against `.ctors' can not be used when making a shared object; recompile with -fPIC
	/usr/lib64/pcc/x86_64-unknown-linux-gnu/1.1.0.DEVEL/lib/crtbegin.o: could not read symbols: Bad value
