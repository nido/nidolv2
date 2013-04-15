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
test_ringbuffer_SOURCES = test_ringbuffer.c
test_ringbuffer_LDADD = ringbuffer.lo


nidoampdir = @lv2dir@/nidoamp.lv2

include $(top_srcdir)/am_include.mk

CLEANFILES=manifest.ttl nidoamp.ttl nidoamp.peg nidoamp/gmon.out
