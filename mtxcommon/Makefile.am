INCLUDES = \
        -I$(top_srcdir)/include \
        @GTK_CFLAGS@ \
	@LIBGLADE_CFLAGS@ \
        @MEGATUNIX_DEFINES@

noinst_LTLIBRARIES = libmtxcommon.la

libmtxcommon_la_SOURCES = crx.c getfiles.c configfile.c xmlbase.c

libmtxcommon_la_LDFLAGS = @EXTRA_LIBS@ @GTK_LIBS@

COMMON_FLAGS = @PROF_FLAGS@ @DEBUG_FLAGS@ 

AM_CFLAGS = $(COMMON_FLAGS) -Wall -Wextra -Wno-unused -pedantic

CLEANFILES = *~ core gmon.out nohup.out 

EXTRA_DIST = 


