AC_INIT([Nidos Plugins], [0.0.1])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([Nidos Plugins], [0.0.1])

AC_LANG_C
AC_PROG_CC
AC_EXEEXT #what is this?


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

AC_CONFIG_FILES([Makefile amp/Makefile])
AC_OUTPUT
