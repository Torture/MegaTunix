INCLUDES = \
        -I$(top_srcdir)/include \
	-I$(top_srcdir)/widgets \
        @GTK_CFLAGS@ \
	@LIBGLADE_CFLAGS@ \
        @MEGATUNIX_DEFINES@


msloader_LDADD = @GTK_LIBS@ ../mtxcommon/libmtxcommon.la @EXTRA_LIBS@ @WIN32_RESOURCES@ @PROF_FLAGS@
mtxloader_LDADD = @GTK_LIBS@ @LIBGLADE_LIBS@ ../mtxcommon/libmtxcommon.la @EXTRA_LIBS@ @WIN32_RESOURCES@ @PROF_FLAGS@

COMMON_FLAGS = @PROF_FLAGS@ @DEBUG_FLAGS@ 

AM_CFLAGS = $(COMMON_FLAGS) -Wall -Wextra -Wno-unused -pedantic
AM_CPPFLAGS = $(COMMON_FLAGS) -Wall -Wextra -Wno-unused -pedantic

CLEANFILES = *~ core gmon.out nohup.out 
DISTCLEANFILES = *~ core gmon.out nohup.out  *.bak *.gladep

msloader_SOURCES = main.c winserialio.c ms1_loader.c ms2_loader.c loader_common.c
mtxloader_SOURCES = mtxloader.c winserialio.c ms1_loader.c ms2_loader.c loader_common.c

msloader_DEPENDENCIES = @WIN32_RESOURCES@  ../mtxcommon/libmtxcommon.la
mtxloader_DEPENDENCIES = @WIN32_RESOURCES@  ../mtxcommon/libmtxcommon.la

resources.o:
	$(WINDRES) -i resources.rc -o resources.o

bin_PROGRAMS = msloader mtxloader

EXTRA_DIST = mtxloader.ico
