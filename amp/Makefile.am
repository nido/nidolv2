lv2_LTLIBRARIES = nidoamp.la

nidoamp_la_SOURCES = nidoamp.c
nidoamp_la_LDFLAGS = -module -lm
lv2_EXTRAS = nidoamp.ttl manifest.ttl

datarootdir = @datarootdir@
lv2dir = @lv2dir@/nidoamp.lv2

%.ttl: %.ttl.in
	cat $< | sed s/@LIB_EXT@/.so/g > $@
