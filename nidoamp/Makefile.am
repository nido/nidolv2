SUFFIXES: .ttl .ttlin

nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_DATA = nidoamp.ttl manifest.ttl

nidoamp_la_SOURCES = nidoamp.c
nidoamp_la_LDFLAGS = -module -lm -avoid-version -shared

nidoampdir = @lv2dir@/nidoamp.lv2

.ttlin.ttl:
	cat $< | sed s/@LIB_EXT@/@SHREXT@/g > $@

CLEANFILES=manifest.ttl
