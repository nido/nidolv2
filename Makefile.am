AUTOMAKE_OPTIONS = foreign dist-bzip2

bin_SCRIPTS = lv2wisdom.sh


#gs  -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=

if MAKE_PDF
pdf_DATA = manual.pdf

documentation/latex/refman.pdf: documentation
	$(MAKE) -C documentation/latex refman.pdf

nidoamp/coverage.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp coverage.pdf
nidoamp/gprof.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp gprof.pdf
nidoamp/gprofpic.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp gprofpic.pdf

manual.pdf: documentation/latex/refman.pdf nidoamp/coverage.pdf nidoamp/gprof.pdf nidoamp/gprofpic.pdf
	gs  -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=manual.pdf documentation/latex/refman.pdf nidoamp/coverage.pdf nidoamp/gprof.pdf nidoamp/gprofpic.pdf

endif #MAKE_PDF

html_DIRDATA = documentation/html

ACLOCAL_AMFLAGS = -I .
SUBDIRS = nidoamp

documentation: Doxyfile README.md
	doxygen $<

documentation/html: documentation
	$(MAKE) $(AM_MAKEFLAGS) documentation

html-am: documentation/html
pdf-am: manual.pdf
EXTRA_DIST = README.md

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing libtool.m4  lt~obsolete.m4  ltoptions.m4  ltsugar.m4  ltversion.m4

install-html-am: $(html_DIRDATA)
	test -z "$<" || cp -r $(html_DIRDATA) "$(DESTDIR)$(htmldir)/html"

CLEANFILES=Doxylog manual.pdf
clean-local:
	rm -rf documentation/

include $(top_srcdir)/am_include.mk
