nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_DATA = nidoamp.ttl manifest.ttl
check_PROGRAMS = test_nidoamp test_ringbuffer
TESTS = $(check_PROGRAMS)
BUILT_SOURCES = nidoamp.peg

nidoamp_la_SOURCES = nidoamp.c nidoamp.h nidoamp.peg ringbuffer.c ringbuffer.h innerproduct.c innerproduct.h cpuid.c cpuid.h
nidoamp_la_LDFLAGS = -module -avoid-version -shared $(AM_LDFLAGS)

test_nidoamp_SOURCES = test_nidoamp.c
test_nidoamp_LDADD = $(am_nidoamp_la_OBJECTS)
test_nidoamp_DEPENDENCIES = nidoamp.la
test_ringbuffer_SOURCES = test_ringbuffer.c ringbuffer.h
test_ringbuffer_LDADD = $(am_nidoamp_la_OBJECTS)


nidoampdir = @lv2dir@/nidoamp.lv2

include $(top_srcdir)/am_include.mk


#if COVERAGE
#$(BOARDS:.mcm=.net)
.gcda.c:
	gcov -a -b -c -f -u -o . $<

baseline.info: $(LTLIBRARIES) $(check_PROGRAMS)
	$(MAKE) $(AM_MAKEFLAGS) check
	geninfo -i -b . . -o baseline.info

# note; this one may need more careful inspection
measure.info: baseline.info $(BUILT_SOUCES)
	./test_nidoamp
	geninfo -b . . -o measure.info
	

total.info: baseline.info measure.info
	lcov -a baseline.info -a measure.info -o total.info

coverage-html: total.info
	genhtml --function-coverage --no-sort --legend -t NidoLV2 -o html --show-details total.info

coverage.pdf: coverage-html
	htmldoc -t pdf14 -f coverage.pdf --color --continuous html/nidolv2/nidoamp/index-detail.html html/nidolv2/nidoamp/*.c.func.* html/nidolv2/nidoamp/*.c.gcov*
	
gmon.out: test_nidoamp $(BUILT_SOURCES)
	./test_nidoamp
	echo "status"

gprof.txt: gmon.out
	gprof test_nidoamp | tee gprof.txt

gprof.ps: gprof.txt
	enscript gprof.txt -p gprof.ps

.ps.pdf:
	ps2pdf $<

gprofpic.dot: gprof.txt
	cat gprof.txt | gprof2dot > gprofpic.dot

.dot.svg: 
	cat $< | dot -Tsvg > $@

.dot.pdf: 
	cat $< | dot -Tpdf > $@

#gprof test_nidoamp | gprof2dot -n 0.05 -e 0.01 | grep -Go label=.*
#endif

CLEANFILES=manifest.ttl nidoamp.ttl nidoamp.peg gmon.out coverage.pdf gprof.pdf gprof.ps gprof.txt gprofpic.dot gprofpic.pdf gprofpic.svg *.gcda *.gcno baseline.info measure.info total.info
clean-local:
	rm -rf html/
