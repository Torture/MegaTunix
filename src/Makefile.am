
INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/widgets \
	-I$(top_srcdir)/mtxmatheval \
	@GTK_CFLAGS@ \
	@GLEXT_CFLAGS@ \
	@LIBGLADE_CFLAGS@ \
	@MEGATUNIX_DEFINES@

bin_PROGRAMS = megatunix

megatunix_SOURCES = \
	Makefile \
	2d_table_editor.c \
	3d_vetable.c \
	about_gui.c \
	afr_calibrate.c \
	apicheck.c \
	args.c \
	comms.c \
	comms_gui.c \
	combo_loader.c \
	combo_mask.c \
	conversions.c \
	core_gui.c \
	dashboard.c \
	dataio.c \
	datamgmt.c \
	datalogging_gui.c \
	debugging.c \
	dep_loader.c \
	dep_processor.c \
	dispatcher.c \
	fileio.c \
	gui_handlers.c \
	helpers.c \
	init.c \
	interrogate.c \
	keybinder.c \
	keyparser.c \
	listmgmt.c \
	locking.c \
	logviewer_core.c \
	logviewer_events.c \
	logviewer_gui.c \
	lookuptables.c \
	main.c \
	menu_handlers.c \
	memory_gui.c \
	mode_select.c \
	ms1-t-logger.c \
	ms2-t-logger.c \
	mtxsocket.c \
	multi_expr_loader.c \
	notifications.c \
	offline.c \
	post_process.c \
	raweditor.c \
	req_fuel.c \
	rtv_map_loader.c \
	rtv_processor.c \
	runtime_gui.c \
	runtime_sliders.c \
	runtime_status.c \
	runtime_text.c \
	serialio.c \
	stringmatch.c \
	tabloader.c \
	tag_loader.c \
	therm.c \
	threads.c \
	timeout_handlers.c \
	user_outputs.c \
	vetable_gui.c \
	vex_support.c \
	visibility.c \
	warmwizard_gui.c \
	watches.c \
	widgetmgmt.c \
	winserialio.c \
	xmlcomm.c 

megatunix_DEPENDENCIES = @WIN32_RESOURCES@  ../widgets/libmtxgauge.la ../widgets/libmtxcurve.la ../widgets/libmtxprogress.la ../widgets/libmtxpie.la ../widgets/libmtxstripchart.la ../mtxcommon/libmtxcommon.la ../mtxmatheval/libmtxmatheval.la

resources.o:
	$(WINDRES) -i resources.rc -o resources.o

megatunix_LDADD = @GTK_LIBS@ @GLEXT_LIBS@ @LIBGLADE_LIBS@ ../mtxmatheval/libmtxmatheval.la -lfl ../mtxcommon/libmtxcommon.la ../widgets/libmtxcurve.la ../widgets/libmtxprogress.la ../widgets/libmtxpie.la ../widgets/libmtxstripchart.la ../widgets/libmtxgauge.la @EXTRA_LIBS@ @WIN32_RESOURCES@ @PROF_FLAGS@

megatunix_LDFLAGS = $(LTLIBINTL)

COMMON_FLAGS = @PROF_FLAGS@ @DEBUG_FLAGS@ 

AM_CFLAGS = $(COMMON_FLAGS) -Wall 

CLEANFILES = *~ core gmon.out nohup.out 

EXTRA_DIST = README NEWS TODO AUTHORS mtx.ico

