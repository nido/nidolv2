AC_INIT([nido-lv2], [0.0.1])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([gnu dist-bzip2 dist-zip])
AC_CONFIG_MACRO_DIR([m4])


AC_LANG_C
AC_PROG_CC
AC_PROG_LIBTOOL

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
CFLAGS="$CFLAGS -g -O0"
CXXFLAGS="$CXXFLAGS -g -O0"
fi


AC_ARG_WITH(lv2dir,
[--with-lv2dir=DIR  Use DIR as the lv2 plugin directory [[LIBDIR/lv2]]],
lv2dir="\${libdir}/$with_datarootdir",
lv2dir='$(libdir)/lv2')
AC_SUBST(lv2dir)

SHREXT=`libtool --config | grep shrext | grep -Go '\.[^"]*'`
AC_SUBST(SHREXT)
AC_CONFIG_FILES([Makefile nidoamp/Makefile])
AC_OUTPUT
