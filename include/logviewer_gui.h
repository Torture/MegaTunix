/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute, etc. this as long as all the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#ifndef __LOGVIEWER_GUI_H__
#define __LOGVIEWER_GUI_H__

#include <gtk/gtk.h>

/* Prototypes */
void build_logviewer(GtkWidget *);
void present_viewer_choices(void *);
gboolean view_value_set(GtkWidget *, gpointer );
gboolean populate_viewer(GtkWidget * );
gboolean lv_expose_event(GtkWidget *, GdkEventExpose *, gpointer );
gboolean lv_configure_event(GtkWidget *, GdkEventConfigure *, gpointer );
/* Prototypes */

#endif