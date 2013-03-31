SUFFIXES: .ttl .ttlin

nidoamp_LTLIBRARIES = nidoamp.la
nidoamp_DATA = nidoamp.ttl manifest.ttl

nidoamp_la_SOURCES = nidoamp.c nidoamp.peg
nidoamp_la_LDFLAGS = -module -lm -avoid-version -shared
BUILT_SOURCES = nidoamp.peg

nidoampdir = @lv2dir@/nidoamp.lv2

.ttl.peg:
	lv2peg $< $@

CLEANFILES=manifest.ttl
