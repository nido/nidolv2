AC_INIT([nido-lv2], [0.0.1])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([gnu dist-bzip2 dist-zip])
AC_CONFIG_MACRO_DIR([m4])


AC_LANG_C
AC_PROG_CC
AC_PROG_LIBTOOL

AC_TYPE_UINT32_T

AC_CHECK_LIB([fftw3], [fftw_plan_dft_r2c_1d],, [AC_MSG_ERROR([fftw3 required])],[]) 


AC_ENABLE_SHARED
LT_INIT([disable-static])
AC_PROG_LN_S

AC_ARG_ENABLE(maintainer-mode, [AS_HELP_STRING([--enable-maintainer-mode], [build with debugging and no optimization])],
[
maintainer_mode=yes
],
[
maintainer_mode=no
]
)

if test "$maintainer_mode" = "yes"; then
CFLAGS="$CFLAGS -g -O0 -Wall -Werror"
CXXFLAGS="$CXXFLAGS -g -O0 -Wall -Werror"
fi

AC_ARG_WITH(fouriersize,
[--with-fouriersize=SIZE  Use SIZE as the size of the fourier transform (default=512)],
[],
[with_fouriersize=512])

AC_DEFINE_UNQUOTED(FOURIER_SIZE, $with_fouriersize)
AC_SUBST(FOURIER_SIZE, $with_fouriersize)

AC_DEFINE_UNQUOTED(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))
AC_SUBST(COMPLEX_SIZE, $(($with_fouriersize / 2 + 1 )))

AC_ARG_WITH(lv2dir,
[--with-lv2dir=DIR  Use DIR as the lv2 plugin directory [[LIBDIR/lv2]]],
lv2dir=$with_lv2dir
# alter libdir so libtool gets the right path in the .la file
libdir=$with_lv2dir,
lv2dir='$(libdir)/lv2')
AC_SUBST(lv2dir)

AC_SUBST([SHREXT],[`/usr/bin/libtool --config | grep shrext | grep -Go '\.[[^"]]*'`])

AC_CONFIG_FILES([Makefile nidoamp/Makefile nidoamp/manifest.ttl nidoamp/nidoamp.ttl lv2wisdom.sh])
AC_OUTPUT
