AUTOMAKE_OPTIONS = foreign dist-bzip2

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing libtool.m4  lt~obsolete.m4  ltoptions.m4  ltsugar.m4  ltversion.m4
maintainer-clean-local:
	rm -rf m4

bin_SCRIPTS = lv2wisdom.sh

ACLOCAL_AMFLAGS = -I .
SUBDIRS = nidoamp
AM_LDFLAGS = @AM_LDFLAGS@
datarootdir = @datarootdir@
