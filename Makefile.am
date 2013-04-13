AUTOMAKE_OPTIONS = foreign dist-bzip2

bin_SCRIPTS = lv2wisdom.sh
pdf_DATA = refman.pdf
html_DIRDATA = documentation/html

ACLOCAL_AMFLAGS = -I .
SUBDIRS = nidoamp

documentation: Doxyfile README.md
	doxygen $<

documentation/latex/refman.pdf: documentation
	$(MAKE) -C documentation/latex refman.pdf

refman.pdf: documentation/latex/refman.pdf
	cp $^ $@

documentation/html: documentation

html-am: documentation
#html: documentation/html
#	cp -r $^ $@

EXTRA_DIST = README.md

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing libtool.m4  lt~obsolete.m4  ltoptions.m4  ltsugar.m4  ltversion.m4

install_HTML: $(HTML_DIRDATA)
	$(INSTALL_DATA) $< "$(DESTDIR)$(htmldir)" || exit $$?

CLEANFILES=Doxylog refman.pdf
clean-local:
	rm -rf documentation/

include $(top_srcdir)/am_include.mk
