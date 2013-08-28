# AC_TYPE_UINT32_T and LT_INIT cause problems in 2.59. 2.69 is the
# lowest version number tested
AC_PREREQ([2.68])
AC_INIT([nidolv2], [0.0.1])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([gnu dist-bzip2 dist-zip])
#AC_CONFIG_MACRO_DIR([.])
AC_CONFIG_MACRO_DIR([m4])

# Options
AC_ARG_ENABLE(maintainer-mode,
	[AS_HELP_STRING([--enable-maintainer-mode],
		[build with debugging and no optimization])
	],[maintainer_mode=yes],[maintainer_mode=no])

AC_ARG_ENABLE(sse3,
	[AS_HELP_STRING([--enable-sse3] [Build with SSE 4.1 support])
	 AS_HELP_STRING([--disable-sse3] [Build Without SSE 4.1 support]) ],
	[sse3=$enableval],
	[])

# mayby something with AX_EXT for "all i can do"

AC_ARG_ENABLE(openmp,
	[AS_HELP_STRING([--enable-openmp]
		[build with OpenMP Multiprocessor support])],
	[openmp=$enableval],
	[openmp=no])

AC_ARG_ENABLE(coverage,
	[AS_HELP_STRING([--enable-coverage]
		[do code coverage checking])],
	[coverage=$enableval],
	[])

AC_ARG_WITH(fouriersize,
	[--with-fouriersize=SIZE  Fourier Transform input size (default=512)],
	[],[with_fouriersize=512])


AC_ARG_WITH([lv2dir],
	[--with-lv2dir=DIR  Use DIR as the lv2 plugin directory [[LIBDIR/lv2]]],
	[lv2dir=$with_lv2dir
	# alter libdir so libtool gets the right path in the .la file
	libdir=$with_lv2dir],
	[lv2dir='$(libdir)/lv2'])

nz_cflags_temp="$CFLAGS"

#languages
AC_LANG_C
AC_PROG_CC

#programs 
AC_CHECK_PROG([CPPCHECK], [cppcheck], [cppcheck], [false])
AC_CHECK_PROG([INDENT],   [indent],   [indent],   [false])
AC_CHECK_PROG([PDFLATEX], [pdflatex], [pdflatex], [false])
AC_CHECK_PROG([DOXYGEN],   [doxygen],   [doxygen],   [false])
AC_CHECK_PROG([DOXYGEN],   [doxygen],   [doxygen],   [false])
AC_CHECK_PROG([DOXYGEN],   [doxygen],   [doxygen],   [false])
AC_CHECK_PROG([LV2PEG],   [lv2peg],   [lv2peg],   [false])
AC_CHECK_PROG([GCOV],   [gcov],   [gcov],   [false])
AC_CHECK_PROG([GENHTML],   [genhtml],   [genhtml],   [false])
AC_CHECK_PROG([GENINFO],   [geninfo],   [geninfo],   [false])
#AC_CHECK_PROG([],   [],   [],   [false])

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
#   AX_CHECK_COMPILE_FLAG(FLAG, [ACTION-SUCCESS], [ACTION-FAILURE], [EXTRA-FLAGS])

if test "$maintainer_mode" = "yes"; then
AC_DEFINE(DEBUG)

test "$nz_cflags_temp" = "" && CFLAGS="$nz_cflags_temp"
AX_CHECK_COMPILE_FLAG([-Wall], [AM_CFLAGS="$AM_CFLAGS -Wall"], [], [])
AX_CHECK_COMPILE_FLAG([-Wextra], [AM_CFLAGS="$AM_CFLAGS -Wextra"], [], [])
AX_CHECK_COMPILE_FLAG([-ggdb], [AM_CFLAGS="$AM_CFLAGS -ggdb"], [GGDB=no], [])
test "$GGDB" = "no" && AX_CHECK_COMPILE_FLAG([-g], [AM_CFLAGS="$AM_CFLAGS -ggdb"], [], [])
AX_CHECK_COMPILE_FLAG([-Og], [AM_CFLAGS="$AM_CFLAGS -Og"], [OG=no], [])
test "$OG" = "no" && AX_CHECK_COMPILE_FLAG([-O0], [AM_CFLAGS="$AM_CFLAGS -Og"], [], [])

fi

if test "$coverage" != "no" -a  "$CC" = "gcc"; then
coverage="yes"
fi

AC_MSG_CHECKING([whether to make a coverage report])
AC_MSG_RESULT([$coverage])

if test "$coverage" = "yes"; then
AM_CFLAGS="$AM_CFLAGS -DNDEBUG"
AX_CHECK_COMPILE_FLAG([-fprofile-arcs],
  [AM_CFLAGS="$AM_CFLAGS -fprofile-arcs"],
  [AC_MSG_ERROR([Compiler does not understand -fprofile-arcs needed for coverage])], []
)
AX_CHECK_COMPILE_FLAG([-ftest-coverage],
  [AM_CFLAGS="$AM_CFLAGS -ftest-coverage"],
  [AC_MSG_ERROR([Compiler does not understand -ftest-coverage needed for coverage])], []
)
	if test GCOV = "false"; then
AC_MSG_ERROR([gcov is needed to do coverage])
	fi
	if test GENHTML = "false"; then
AC_MSG_ERROR([genhtml (from lcov) is required to report on coverage])
	fi
	if test GENINFO = "false"; then
AC_MSG_ERROR([geninfo (from lcov) is required to report on coverage])
	fi
fi

if test "$sse3" = "yes"; then
	AX_CHECK_COMPILE_FLAG([-msse3],
		[AM_CFLAGS="$AM_CFLAGS -msse3"],
		[AC_MSG_ERROR([SSE 4.1 support requested but not suported by compiler]) ]
	)
fi

if test "$openmp" = "yes"; then
	AX_CHECK_COMPILE_FLAG([-fopenmp],
		[AM_CFLAGS="$AM_CFLAGS -fopenmp"],
		[AC_ERROR([Compiler does not understand -fopenmp and --enable-openmp is defined])]
	)
if test "$CC" = "clang"; then
AC_MSG_ERROR([clang does not understand openmp properly according to field tests])
fi
AC_DEFINE(__OPENMP__)
fi

#library functions
AC_CHECK_LIB([fftw3f], [fftwf_plan_dft_r2c_1d],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_plan_dft_c2r_1d],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_malloc],, [AC_MSG_ERROR([fftw3 float version required])],[]) 
AC_CHECK_LIB([fftw3f], [fftwf_execute],, [AC_MSG_ERROR([fftw3 float version required])],[]) 

#system services

AC_ENABLE_SHARED
LT_INIT([disable-static dlopen])

# information
if test "$LV2PEG" = "false"; then
	AC_MSG_ERROR([Cannot find lv2peg needed to create pegfile])
fi

if test -z "$DOXYGEN"; then
AC_MSG_WARN([Cannot build documentation, need doxygen])
fi
AM_CONDITIONAL([MAKE_DOC], [test $DOXYGEN != false])

if test -z "$PDFLATEX"; then
AC_MSG_WARN([Cannot build pdf documentation])
fi
AM_CONDITIONAL([MAKE_PDF], [test $PDFLATEX != false])
AM_CONDITIONAL([COVERAGE], [test "$coverage" = yes])

# macro redefinition
AC_DEFINE_UNQUOTED(FOURIER_SIZE, $with_fouriersize)
AC_SUBST(FOURIER_SIZE, $with_fouriersize)

AC_DEFINE_UNQUOTED(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))
AC_SUBST(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))

AC_SUBST(lv2dir)

AC_SUBST([SHREXT],[`/usr/bin/libtool --config | grep shrext | grep -Go '\.[[^"]]*'`])

AC_SUBST(AM_CFLAGS)

AC_SUBST(GENHTML)
AC_SUBST(GENINFO)



AC_CONFIG_FILES([Makefile nidoamp/manifest.ttl nidoamp/nidoamp.ttl lv2wisdom.sh Doxyfile])
AC_OUTPUT
