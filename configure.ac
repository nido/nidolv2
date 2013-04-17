# AC_TYPE_UINT32_T and LT_INIT cause problems in 2.59. 2.69 is the
# lowest version number tested
AC_PREREQ([2.68])
AC_INIT([nidolv2], [0.0.1])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([gnu dist-bzip2 dist-zip])
AC_CONFIG_MACRO_DIR([.])

# Options
AC_ARG_ENABLE(maintainer-mode,
	[AS_HELP_STRING([--enable-maintainer-mode],
		[build with debugging and no optimization])
	],[maintainer_mode=yes],[maintainer_mode=no])

AC_ARG_ENABLE(sse41,
	[AS_HELP_STRING([--enable-sse41] [Build with SSE 4.1 support])
	 AS_HELP_STRING([--disable-sse41] [Build Without SSE 4.1 support]) ],
	[sse41=$enableval],
	[])

# mayby something with AX_EXT for "all i can do"

AC_ARG_ENABLE(openmp,
	[AS_HELP_STRING([--enable-openmp]
		[build with OpenMP Multiprocessor support])],
	[openmp=yes],
	[openmp=no])



AC_ARG_WITH(fouriersize,
	[--with-fouriersize=SIZE  Fourier Transform input size (default=512)],
	[],[with_fouriersize=512])


AC_ARG_WITH([lv2dir],
	[--with-lv2dir=DIR  Use DIR as the lv2 plugin directory [[LIBDIR/lv2]]],
	[lv2dir=$with_lv2dir
	# alter libdir so libtool gets the right path in the .la file
	libdir=$with_lv2dir],
	[lv2dir='$(libdir)/lv2'])

#languages
AC_LANG_C
AC_PROG_CC

#programs 
AC_CHECK_PROG([CPPCHECK], [cppcheck], [cppcheck], [false])
AC_CHECK_PROG([PDFLATEX], [pdflatex], [pdflatex], [false])
AC_CHECK_PROG([LV2PEG],   [lv2peg],   [lv2peg],   [false])
if test "$LV2PEG" = "false"; then
	AC_MSG_ERROR([Cannot find lv2peg])
fi
AC_CHECK_PROG([INDENT],   [indent],   [indent],   [false])

AC_PROG_LN_S
AC_PROG_LIBTOOL

#libraries
AC_SEARCH_LIBS([cos], [m], [], AC_MSG_ERROR([Cannot find cos() function in libm]), [])

#header files
AC_CHECK_HEADERS(assert.h complex.h math.h stdbool.h stddef.h stdio.h stdlib.h strings.h, [],
	[AC_MSG_ERROR([Cannot find standard include headers])],[]
)
AC_CHECK_HEADER([lv2.h], [], [AC_MSG_ERROR([need lv2 header file])], [])
AC_CHECK_HEADER([fftw3.h], [], [AC_MSG_ERROR([need fftw3 header file])], [])

#types
AC_TYPE_UINT32_T

#structures

#compiler specifics
<<<<<<< HEAD
AM_CFLAGS="$AM_CFLAGS -Wall"
=======

AM_CFLAGS="$AM_CFLAGS -Wall"

if test "$maintainer_mode" = "yes"; then
AM_CFLAGS="$AM_CFLAGS -O0 -g -pg"
AM_LDFLAGS="$AM_LIBFLAGS /lib/gcrt1.o"
fi

if test "$sse41" = "yes"; then
	AX_CHECK_COMPILE_FLAG(-msse4.1,
		[AM_CFLAGS="$AM_CFLAGS -msse4.1"],
		[AC_MSG_ERROR([SSE 4.1 support requested but not suported by compiler]) ]
	)
fi

if test "$openmp" = "yes"; then
	AX_CHECK_COMPILE_FLAG([-fopenmp],
		[AM_CFLAGS="$AM_CFLAGS -fopenmp"],
		[AC_ERROR([Compiler does not understand -fopenmp and --enable-openmp is defined])]
	)
AC_DEFINE(__OPENMP__)
fi
>>>>>>> 2babca6b7680b70bd5dd77234e83cc198ef98712


#library functions
AC_CHECK_LIB([fftw3f], [fftwf_plan_dft_r2c_1d],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_plan_dft_c2r_1d],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_malloc],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_execute],, [AC_MSG_ERROR([fftw3 float version required])],[]) 

#system services

AC_ENABLE_SHARED
LT_INIT([disable-static dlopen])

<<<<<<< HEAD
#am conditionals
AM_CONDITIONAL([MAKE_PDF], [test x$PDFLATEX != x])
AM_CONDITIONAL([COVERAGE], [test "$maintainer_mode" != "no"])
=======

>>>>>>> 2babca6b7680b70bd5dd77234e83cc198ef98712
# information
if test -z "$PDFLATEX"; then
AC_MSG_WARN([Cannot build pdf documentation])
fi
AM_CONDITIONAL([MAKE_PDF], [test x$PDFLATEX != x])

# macro redefinition
AC_DEFINE_UNQUOTED(FOURIER_SIZE, $with_fouriersize)
AC_SUBST(FOURIER_SIZE, $with_fouriersize)

AC_DEFINE_UNQUOTED(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))
AC_SUBST(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))

AC_SUBST(lv2dir)

AC_SUBST([SHREXT],[`/usr/bin/libtool --config | grep shrext | grep -Go '\.[[^"]]*'`])

if test "$maintainer_mode" = "yes"; then
AM_CFLAGS="$AM_CFLAGS -fprofile-arcs -ftest-coverage -g -pg"
AM_LDFLAGS="/lib/gcrt1.o"
# take out the -O2 bit if you are going to debug
CFLAGS=`echo $CFLAGS | sed "s/-O2/-O0/g"`
#LIBS="$LIBS -lc_p"
AC_DEFINE(DEBUG)
fi

AC_SUBST(AM_CFLAGS)
AC_CONFIG_FILES([Makefile nidoamp/Makefile nidoamp/manifest.ttl nidoamp/nidoamp.ttl lv2wisdom.sh Doxyfile])
AC_OUTPUT
