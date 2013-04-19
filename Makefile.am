AUTOMAKE_OPTIONS = foreign dist-bzip2

bin_SCRIPTS = lv2wisdom.sh
check_PROGRAMS = test_nidoamp test_ringbuffer
TESTS = $(check_PROGRAMS)
BUILT_SOURCES = nidoamp/nidoamp.peg

nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_la_SOURCES = nidoamp/nidoamp.c nidoamp/nidoamp.h nidoamp/nidoamp.peg nidoamp/ringbuffer.c nidoamp/ringbuffer.h nidoamp/innerproduct.c nidoamp/innerproduct.h nidoamp/cpuid.c nidoamp/cpuid.h
nidoamp_la_LDFLAGS = -module -avoid-version -shared $(AM_LDFLAGS)
nidoamp_DATA = nidoamp/nidoamp.ttl nidoamp/manifest.ttl

test_nidoamp_SOURCES = nidoamp/test_nidoamp.c
#EXTRA_test_nidoamp_SOURCES = nidoamp/sse41.c # do this once sse is "properly" installed
test_nidoamp_LDADD = $(am_nidoamp_la_OBJECTS)
test_nidoamp_DEPENDENCIES = nidoamp.la

test_ringbuffer_SOURCES = nidoamp/test_ringbuffer.c
test_ringbuffer_LDADD = ringbuffer.lo
test_ringbuffer_DEPENDENCIES = nidoamp.la

#Documentation
if MAKE_DOC
html_DATA = doxygen/html/*

if MAKE_PDF
pdf_DATA = doxygen/latex/refman.pdf
doxygen/latex/refman.pdf: doxygen
	$(MAKE) $(AM_MAKEFLAGS) -C doxygen/latex refman.pdf
endif # MAKE_PDF

endif # MAKE_DOC

#if MAKE_CALL
##valgrind --tool=callgrind --callgrind-out-file=test_nidoamp.callgrind ./test_nidoamp && gprof2dot -f callgrind test_nidoamp.callgrind | dot -Tpdf > out.pd
%.callgraph: %
	valgrind --tool=callgrind --callgrind-out-file=$@ ./$<

.callgraph.dot:
	gprof2dot -o$@ -f callgrind $<

.dot.pdf:
	dot -Tpdf -o$@ $<

callgraph: $(check_PROGRAMS)
	$(MAKE) $(AM_MAKEFLAGS) $(addsuffix .pdf,$^)

#endif #MAKE_CALL

if MAKE_COV
%.info: %
	lcov -z -d .
	lcov -c -i -d . -o baseline-$@
	./`echo $< | sed "s/.gcno//"`
	lcov -c -d . -t `echo $< | sed "s/.gcno//"` -o measure-$@
	lcov -a baseline-$@ -a measure-$@ -o total-$@
	lcov -e total-$@ "$(abs_top_srcdir)/*" -o $@

coverage: $(addsuffix .info,$(check_PROGRAMS))
	$(MAKE) $(AM_MAKEFLAGS) $^
	genhtml -s -t $(PACKAGE) --legend --sort -o coverage $^
endif # MAKE_COV

doxygen/html/*: doxygen
doxygen: Doxyfile.in
	$(MAKE) $(AM_MAKEFLAGS) Doxyfile
	doxygen

%.valgrind: %
	valgrind --leak-check=full -v ./$< 2>$@

valgrind: $(addsuffix .valgrind,$(check_PROGRAMS))

indent:  $(SOURCES)
	test -z "$(SOURCES)" || indent -kr -nut -ts4 -d0 $^


cppcheck:  $(SOURCES)
	test -z "$(SOURCES)" || @CPPCHECK@ --error-exitcode=2 $^

.ttl.peg:
	@LV2PEG@ $< $@

nidoampdir = @lv2dir@/nidoamp.lv2

CLEANFILES=Doxylog nidoamp/manifest.ttl nidoamp/nidoamp.ttl nidoamp/nidoamp.peg nidoamp/gmon.out *.info .libs/*.info *.gcda .libs/*.gcda *.gcno .libs/*.gcno
clean-local:
	rm -rf documentation/

MAINTAINERCLEANFILES=Makefile.in aclocal.m4 nidoamp/Makefile.in config.guess config.sub configure depcomp install-sh ltmain.sh missing libtool.m4  lt~obsolete.m4  ltoptions.m4  ltsugar.m4  ltversion.m4

.PHONY: indent indent-recurse indent-local
