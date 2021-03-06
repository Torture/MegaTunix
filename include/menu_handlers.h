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

#ifndef __MENU_HANDLERS_H__
#define __MENU_HANDLERS_H__

#include <defines.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

/* Prototypes */
void setup_menu_handlers_pf();
EXPORT gboolean jump_to_tab(GtkWidget *, gpointer );
EXPORT gboolean settings_transfer(GtkWidget *, gpointer );
gboolean check_tab_existance(TabIdent );
EXPORT gboolean show_tps_calibrator_window(GtkWidget *, gpointer);
EXPORT gboolean show_ms2_afr_calibrator_window(GtkWidget *, gpointer);
EXPORT gboolean show_table_generator_window(GtkWidget *, gpointer);
EXPORT gboolean show_sensor_calibration_help(GtkWidget *, gpointer);
EXPORT gboolean ms2_reinit(GtkWidget *, gpointer);
EXPORT gboolean ms2_reboot(GtkWidget *, gpointer);
gdouble linear_interpolate(gdouble, gdouble, gdouble, gdouble, gdouble);

/* Prototypes */

#endif
