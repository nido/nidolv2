AUTOMAKE_OPTIONS = foreign dist-bzip2

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing m4/libtool.m4  m4/lt~obsolete.m4  m4/ltoptions.m4  m4/ltsugar.m4  m4/ltversion.m4
maintainer-clean-local:
	rm -rf m4

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = nidoamp
AM_LDFLAGS = @AM_LDFLAGS@
datarootdir = @datarootdir@
