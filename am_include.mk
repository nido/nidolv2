indent: indent-recurse indent-local
cppcheck: cppcheck-recurse cppcheck-local

indent-recurse: $(SUBDIRS)
	for dir in $(SUBDIRS); \
	do cd $$dir && $(MAKE) $(AM_MAKEFLAGS) indent; \
	done

cppcheck-recurse: $(SUBDIRS)
	for dir in $(SUBDIRS); \
	do cd $$dir && $(MAKE) $(AM_MAKEFLAGS) cppcheck; \
	done

indent-local:  $(SOURCES)
	test -z "$(SOURCES)" || indent -kr -nut -ts4 -d0 $^

cppcheck-local:  $(SOURCES)
	test -z "$(SOURCES)" || @CPPCHECK@ --error-exitcode=2 $^


.ttl.peg:
	lv2peg $< $@

.PHONY: indent indent-recurse indent-local
