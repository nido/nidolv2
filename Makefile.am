AUTOMAKE_OPTIONS = foreign dist-bzip2
ACLOCAL_AMFLAGS = -I m4

bin_SCRIPTS = lv2wisdom.sh
check_PROGRAMS = test_nidoamp test_ringbuffer
TESTS = $(check_PROGRAMS)
BUILT_SOURCES = nidoamp/nidoamp.peg

nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_la_SOURCES = nidoamp/nidoamp.c nidoamp/nidoamp.h nidoamp/ringbuffer.c nidoamp/ringbuffer.h nidoamp/innerproduct.c nidoamp/innerproduct.h nidoamp/cpuid.c nidoamp/cpuid.h $(BUILT_SOURCES)
nidoamp_la_CFLAGS = -I$(top_builddir)/nidoamp
nidoamp_la_LDFLAGS = -module -avoid-version -shared $(AM_LDFLAGS)
nidoamp_DATA = nidoamp/nidoamp.ttl nidoamp/manifest.ttl
EXTRA_DIST  = README.md

test_nidoamp_SOURCES = nidoamp/test_nidoamp.c
#EXTRA_test_nidoamp_SOURCES = nidoamp/sse41.c # do this once sse is "properly" installed
test_nidoamp_LDADD = $(am_nidoamp_la_OBJECTS)
test_nidoamp_DEPENDENCIES = nidoamp.la

test_ringbuffer_SOURCES = nidoamp/test_ringbuffer.c
test_ringbuffer_LDADD = $(am_nidoamp_la_OBJECTS)
test_ringbuffer_DEPENDENCIES = nidoamp.la

#Documentation
if MAKE_DOC
html_DATA = doxygen/html/*

doxygen/html/*: doxygen
doxygen: Doxyfile.in $(SOURCES) README.md
	$(MAKE) $(AM_MAKEFLAGS) Doxyfile
	doxygen

if MAKE_PDF
pdf_DATA = doxygen/latex/refman.pdf
doxygen/latex/refman.pdf: doxygen
	$(MAKE) $(AM_MAKEFLAGS) -C doxygen/latex refman.pdf
endif # MAKE_PDF

endif # MAKE_DOC

nidoamp/coverage.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp coverage.pdf
nidoamp/gprof.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp gprof.pdf
nidoamp/gprofpic.pdf:
	$(MAKE) $(AM_MAKEFLAGS) -C nidoamp gprofpic.pdf

manual.pdf: documentation/latex/refman.pdf nidoamp/coverage.pdf nidoamp/gprof.pdf nidoamp/gprofpic.pdf
	gs  -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=manual.pdf documentation/latex/refman.pdf nidoamp/coverage.pdf nidoamp/gprof.pdf nidoamp/gprofpic.pdf

#if MAKE_CALL
##valgrind --tool=callgrind --callgrind-out-file=test_nidoamp.callgrind ./test_nidoamp && gprof2dot -f callgrind test_nidoamp.callgrind | dot -Tpdf > out.pd
callgraph/%.callgrind: %
	valgrind --tool=callgrind --callgrind-out-file=$@ ./$<

.callgrind.dot:
	gprof2dot -o$@ -f callgrind $<

.dot.pdf:
	dot -Tpdf -o$@ $<

check-local: cppcheck strict-cppcheck

callgraph: $(check_PROGRAMS)
	rm -rf callgraph && mkdir callgraph
	$(MAKE) $(AM_MAKEFLAGS) $(addprefix callgraph/,$(addsuffix .pdf,$^))

#endif #MAKE_CALL

if COVERAGE
%.info: %
	lcov -z -d .
	lcov -c -i -d . -o baseline-$@
	./$<
	lcov -c -d . -t $< -o measure-$@
	lcov -a baseline-$@ -a measure-$@ -o total-$@
	lcov -e total-$@ "$(abs_top_srcdir)/*" -o $@

coverage: $(BUILT_SOURCES) $(check_PROGRAMS)
	rm -rf coverage/
	$(MAKE) $(AM_MAKEFLAGS) $(addsuffix .info,$(check_PROGRAMS))
	genhtml -s -t $(PACKAGE) --legend --sort -o coverage  $(addsuffix .info,$(check_PROGRAMS))
endif # COVERAGE

%.valgrind: %
	valgrind --leak-check=full -v ./$< 2>$@

valgrind:  $(BUILT_SOURCES) $(addsuffix .valgrind,$(check_PROGRAMS))

indent: $(SOURCES)
	test -z "$(SOURCES)" || indent -kr -nut -ts4 -d0 $^

indent-check: $(SOURCES)
	test -z "$^" || for x in $^; do indent -kr -nut -ts4 -d0 $$x -o $$x.indent; diff $$x $$x.indent && rm $$x.indent || exit; done

cppcheck: $(SOURCES)
	test -z "$^" || @CPPCHECK@ --check-config --quiet --error-exitcode=2 $(filter-out $(BUILT_SOURCES),$^)

strict-cppcheck: $(SOURCES)
	test -z "$^" || @CPPCHECK@ --check-config --quiet --enable=all --error-exitcode=2 $(filter-out $(BUILT_SOURCES),$^)

.ttl.peg:
	@LV2PEG@ $< $@

nidoampdir = @lv2dir@/nidoamp.lv2

CLEANFILES=Doxylog nidoamp/manifest.ttl nidoamp/nidoamp.ttl nidoamp/nidoamp.peg nidoamp/gmon.out *.info .libs/*.info *.gcda .libs/*.gcda *.gcno .libs/*.gcno
distclean-local:
	rm -rf coverage/
	rm -rf doxygen/

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing libtool.m4  lt~obsolete.m4  ltoptions.m4  ltsugar.m4  ltversion.m4

.PHONY: indent indent-recurse indent-local
