AC_DEFUN([NZ_ARG_SET],
	[AC_ARG_ENABLE($1,
        	[AS_HELP_STRING([--enable-$1]
                	[build with $1 support])],
	        [$1=$enableval],
		[m4_if([$2], [], [$1=no], [$1=$2])])])
