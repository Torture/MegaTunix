#include <config.h>
#include <defines.h>
#include <alerts.h>
#include <events.h>
#include <loadsave.h>
#include <handlers.h>
#include <gauge.h>
#include <getfiles.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <polygons.h>
#include <tblocks.h>
#include <tgroups.h>
#include <warnings.h>

GtkWidget * gauge = NULL;
gboolean hold_handlers = FALSE;
GdkColor red = { 0, 65535, 0, 0};
GdkColor black = { 0, 0, 0, 0};
GdkColor white = { 0, 65535, 65535, 65535};
extern gboolean direct_path;
extern gboolean changed;
extern gboolean gauge_loaded;
extern GtkWidget *main_window;



EXPORT gboolean create_new_gauge(GtkWidget * widget, gpointer data)
{
	GtkWidget *tmp = NULL;
	GladeXML *xml = glade_get_widget_tree(widget);
	GtkWidget *parent = glade_xml_get_widget(xml,"gauge_frame");
	gauge = mtx_gauge_face_new();
	gtk_widget_set_events(gauge,GDK_POINTER_MOTION_HINT_MASK
			| GDK_POINTER_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK);
	g_signal_connect(GTK_WIDGET(gauge),"motion_notify_event",G_CALLBACK(gauge_motion),NULL);
	g_signal_connect(GTK_WIDGET(gauge),"button_press_event",G_CALLBACK(gauge_button),NULL);
	gtk_container_add(GTK_CONTAINER(parent),gauge);
	gtk_widget_show_all(parent);
	mtx_gauge_face_redraw_canvas(MTX_GAUGE_FACE(gauge));
	gauge_loaded = TRUE;

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"tab_notebook"),TRUE);
	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"animate_frame"),TRUE);

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"new_gauge_menuitem"),FALSE);

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"close_gauge_menuitem"),TRUE);

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"load_gauge_menuitem"),FALSE);

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"save_gauge_menuitem"),TRUE);

	gtk_widget_set_sensitive(glade_xml_get_widget(xml,"save_as_menuitem"),TRUE);

	update_attributes();
	return (TRUE);
}



EXPORT gboolean close_current_gauge(GtkWidget * widget, gpointer data)
{
	GtkWidget *tmp = NULL;
	GladeXML *xml = glade_get_widget_tree(widget);
	GtkWidget *parent = glade_xml_get_widget(xml,"gauge_frame");

	if (GTK_IS_WIDGET(gauge))
	{
		if (changed)
			prompt_to_save();
		gtk_widget_destroy(gauge);
		gtk_widget_set_sensitive(glade_xml_get_widget(xml,"tab_notebook"),FALSE);
		gauge_loaded = FALSE;
		changed = FALSE;
	}
	gauge = NULL;

	tmp = glade_xml_get_widget(xml,"animate_frame");
	gtk_widget_set_sensitive(tmp,FALSE);

	tmp = glade_xml_get_widget(xml,"new_gauge_menuitem");
	gtk_widget_set_sensitive(tmp,TRUE);

	tmp = glade_xml_get_widget(xml,"load_gauge_menuitem");
	gtk_widget_set_sensitive(tmp,TRUE);

	tmp = glade_xml_get_widget(xml,"close_gauge_menuitem");
	gtk_widget_set_sensitive(tmp,FALSE);

	tmp = glade_xml_get_widget(xml,"save_gauge_menuitem");
	gtk_widget_set_sensitive(tmp,FALSE);

	tmp = glade_xml_get_widget(xml,"save_as_menuitem");
	gtk_widget_set_sensitive(tmp,FALSE);

	tmp = glade_xml_get_widget(xml,"animate_button");
	gtk_widget_set_sensitive(tmp,TRUE);

	reset_onscreen_controls();
	direct_path = FALSE;
	gtk_widget_show_all(parent);
	return (TRUE);
}


void reset_onscreen_controls(void)
{
	reset_text_controls();
	reset_general_controls();
	reset_onscreen_w_ranges();
	reset_onscreen_a_ranges();
	reset_onscreen_tblocks();
	reset_onscreen_tgroups();
	reset_onscreen_polygons();
	return;
}

void update_attributes(void)
{
	update_text_controls();
	update_general_controls();
	update_onscreen_w_ranges();
	update_onscreen_a_ranges();
	update_onscreen_tblocks();
	update_onscreen_tgroups();
	update_onscreen_polygons();
	return;
}

EXPORT gboolean entry_change_color(GtkWidget * widget, gpointer data)
{
        gtk_widget_modify_text(widget,GTK_STATE_NORMAL,&red);
	return TRUE;

}

EXPORT gboolean change_font(GtkWidget *widget, gpointer data)
{
	gchar * tmpbuf = NULL;
	MtxGaugeFace *g = NULL;
	tmpbuf = (gchar *)gtk_font_button_get_font_name (GTK_FONT_BUTTON(widget));
	/* Strip out the font size as the gauge lib uses a different scaling
	 * method that scales with the size of the gauge
	 */
	tmpbuf = g_strchomp(g_strdelimit(tmpbuf,"0123456789",' '));

	if (GTK_IS_WIDGET(gauge))
		g = MTX_GAUGE_FACE(gauge);
	else 
		return FALSE;

	if (hold_handlers)
		return TRUE;

	changed = TRUE;
	mtx_gauge_face_set_value_font(g, tmpbuf);
	return TRUE;
}


EXPORT gboolean color_button_color_set(GtkWidget *widget, gpointer data)
{
	GdkColor color;
	gint handler = (GINT)OBJ_GET((widget),"handler");

	if (!GTK_IS_WIDGET(gauge))
		return FALSE;

	if (hold_handlers)
		return TRUE;

	gtk_color_button_get_color(GTK_COLOR_BUTTON(widget),&color);
	changed = TRUE;
	mtx_gauge_face_set_color(MTX_GAUGE_FACE(gauge),handler,color);

	return TRUE;
}


EXPORT gboolean link_range_spinners(GtkWidget *widget, gpointer data)
{
	GladeXML *xml = glade_get_widget_tree(widget);
	GtkAdjustment *adj = NULL;
	GtkWidget *upper_spin = glade_xml_get_widget(xml,"range_highpoint_spin");

	adj = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(upper_spin));
	adj->lower = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
	if (adj->value < adj->lower)
		adj->value = adj->lower;
	gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(upper_spin),adj);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(upper_spin),adj->value);
	return FALSE;
}



EXPORT gboolean animate_gauge(GtkWidget *widget, gpointer data)
{
	gfloat lower = 0.0;
	gfloat upper = 0.0;

	if (!GTK_IS_WIDGET(gauge))
		return FALSE;

	gtk_widget_set_sensitive(widget,FALSE);

	mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), LBOUND, &lower);
	mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), UBOUND, &upper);
	mtx_gauge_face_set_value(MTX_GAUGE_FACE (gauge),lower);
	g_timeout_add(20,(GtkFunction)sweep_gauge, (gpointer)gauge);
	return TRUE;
}

gboolean sweep_gauge(gpointer data)
{
	static gfloat lower = 0.0;
	static gfloat upper = 0.0;
	gfloat interval = 0.0;
	gfloat cur_val = 0.0;
	GladeXML *xml = NULL;
	GtkWidget *button = NULL;
	static gboolean rising = TRUE;
	GtkWidget * gauge = NULL;

	gauge = (GtkWidget *)data;

	if (!GTK_IS_WIDGET(gauge))
		return FALSE;

	mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), LBOUND, &lower);
	mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), UBOUND, &upper);
	interval = (upper-lower)/75.0;
	mtx_gauge_face_get_value(MTX_GAUGE_FACE (gauge), &cur_val);
	if (cur_val >= upper)
		rising = FALSE;
	if (cur_val <= lower)
		rising = TRUE;

	if (rising)
		cur_val+=interval;
	else
		cur_val-=interval;

	mtx_gauge_face_set_value (MTX_GAUGE_FACE (gauge),cur_val);
	if (cur_val <= lower)
	{
		/* This cancels the timeout once one full complete sweep
		 * of the gauge
		 */
		xml = glade_get_widget_tree(gauge->parent);
		button = glade_xml_get_widget(xml,"animate_button");
		gtk_widget_set_sensitive(button,TRUE);
		mtx_gauge_face_set_value (MTX_GAUGE_FACE (gauge),lower);
		return FALSE;
	}
	else
		return TRUE;

}


EXPORT gboolean grab_coords_event(GtkWidget *widget, gpointer data)
{
	gdouble x = 0.0;
	gdouble y = 0.0;
	gtk_widget_set_sensitive(OBJ_GET(widget,"x_spin"),FALSE);
	gtk_widget_set_sensitive(OBJ_GET(widget,"y_spin"),FALSE);
	gtk_widget_set_sensitive(widget,FALSE);
	OBJ_SET(gauge,"x_spin",OBJ_GET(widget,"x_spin"));
	OBJ_SET(gauge,"y_spin",OBJ_GET(widget,"y_spin"));
	OBJ_SET(gauge,"edit_but",widget);
	return TRUE;

}


gboolean gauge_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	GtkWidget *x_spin = NULL;
	GtkWidget *y_spin = NULL;
	GtkWidget *x_o_spin = NULL;
	GtkWidget *y_o_spin = NULL;
	gfloat x_origin = 0.0;
	gfloat y_origin = 0.0;
	x_spin = (GtkWidget *)OBJ_GET(widget,"x_spin");
	y_spin = (GtkWidget *)OBJ_GET(widget,"y_spin");
	if ((!GTK_IS_WIDGET(x_spin)) || (!GTK_IS_WIDGET(y_spin)))
		return FALSE;
	if ((GBOOLEAN)OBJ_GET(x_spin,"relative"))
	{
		x_o_spin = (GtkWidget *)OBJ_GET(x_spin,"x_o_spin");
		y_o_spin = (GtkWidget *)OBJ_GET(x_spin,"y_o_spin");
		x_origin = gtk_spin_button_get_value(GTK_SPIN_BUTTON(x_o_spin));
		y_origin = gtk_spin_button_get_value(GTK_SPIN_BUTTON(y_o_spin));
		if (GTK_IS_WIDGET(x_spin))
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(x_spin),((event->x-(widget->allocation.width/2.0))/(widget->allocation.width/2.0))-x_origin);
		if (GTK_IS_WIDGET(y_spin))
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(y_spin),((event->y-(widget->allocation.height/2.0))/(widget->allocation.height/2.0))-y_origin);
	}
	else
	{
		if (GTK_IS_WIDGET(x_spin))
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(x_spin),(event->x-(widget->allocation.width/2.0))/(widget->allocation.width/2.0));
		if (GTK_IS_WIDGET(y_spin))
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(y_spin),(event->y-(widget->allocation.height/2.0))/(widget->allocation.height/2.0));
	}
	return TRUE;
}


gboolean gauge_button(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	GtkWidget * tmp = NULL;

	tmp = OBJ_GET(gauge,"x_spin");
	if (GTK_IS_WIDGET(tmp))
		gtk_widget_set_sensitive(tmp,TRUE);
	tmp = OBJ_GET(gauge,"y_spin");
	if (GTK_IS_WIDGET(tmp))
		gtk_widget_set_sensitive(tmp,TRUE);
	tmp = OBJ_GET(gauge,"edit_but");
	if (GTK_IS_WIDGET(tmp))
		gtk_widget_set_sensitive(tmp,TRUE);
	OBJ_SET(gauge,"x_spin",NULL);
	OBJ_SET(gauge,"y_spin",NULL);
	OBJ_SET(gauge,"edit_but",NULL);
	return FALSE;
}
