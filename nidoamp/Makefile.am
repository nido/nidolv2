SUFFIXES: .ttl .ttlin

nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_DATA = nidoamp.ttl manifest.ttl
check_PROGRAMS = test_nidoamp
TESTS = $(check_PROGRAMS)

nidoamp_la_SOURCES = nidoamp.c nidoamp.h nidoamp.peg
nidoamp_la_LDFLAGS = -module -lm -avoid-version -shared

test_nidoamp_SOURCES = test_nidoamp.c
test_nidoamp_LDFLAGS = -lm
test_nidoamp_LDADD = nidoamp.o

BUILT_SOURCES = nidoamp.peg

nidoampdir = @lv2dir@/nidoamp.lv2

.ttl.peg:
	lv2peg $< $@

CLEANFILES=manifest.ttl
