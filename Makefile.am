AUTOMAKE_OPTIONS = foreign dist-bzip2
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = amp
AM_LDFLAGS = @AM_LDFLAGS@
datarootdir = @datarootdir@
#lv2dir = @lv2dir@


MAINTAINERCLEANFILES=Makefile.in aclocal.m4 amp/Makefile.in autom4te.cache/ config.guess config.sub configure depcomp install-sh ltmain.sh missing

.PHONY: convenience-link clean-convenience-link

convenience-link: $(lib_LTLIBRARIES)
	@for soname in `echo | $(EGREP) "^dlname=" $^ | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
	  echo "$$soname: creating convenience link from $(abs_builddir)/.libs to $(top_builddir)"; \
	  rm -f $(top_builddir)/$$soname ; \
	  test -e $(abs_builddir)/.libs/$$soname && \
	  cd $(top_builddir) && \
	  $(LN_S) $(abs_builddir)/.libs/$$soname $$soname || true;\
	done 

clean-convenience-link:
	@for soname in `echo | $(EGREP) "^dlname=" $(lib_LTLIBRARIES) | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
	  echo "$$soname: cleaning convenience links"; \
	  test -L $(top_builddir)/$$soname && rm -f $(top_builddir)/$$soname || true; \
	done 

all-local:: convenience-link

clean-local:: clean-convenience-link
