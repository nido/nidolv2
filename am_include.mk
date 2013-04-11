indent: indent-recurse indent-local

indent-recurse: $(SUBDIRS)
	for dir in $(SUBDIRS); \
	do cd $$dir && $(MAKE) $(AM_MAKEFLAGS) indent; \
	done;

indent-local:  $(SOURCES)
	test -z "$(SOURCES)" || indent -kr -nut -ts4 -d0 $^

.PHONY: indent
