/*
 * Copyright (C) 2003 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 *
 * Linux Megasquirt tuning software
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 * 
 * A portion of this code in this file is a near copy from MegaTune 2.25
 * file genTherm.cpp, copyright Eric Fahlgren.
 *
 * No warranty is made or implied. You use this program at your own risk.
 */

#include <config.h>
#include <defines.h>
#include <enums.h>
#include <firmware.h>
#include <gui_handlers.h>
#include <math.h>
#include <therm.h>
#include <threads.h>
#include <vex_support.h>
#include <widgetmgmt.h>


extern GObject *global_data;



EXPORT gboolean flip_table_gen_temp_label(GtkWidget *widget, gpointer data)
{
	GtkWidget *temp_label = lookup_widget("temp_label");

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) /* Deg C */
		gtk_label_set_text(GTK_LABEL(temp_label),OBJ_GET(temp_label,"c_label"));
	else
		gtk_label_set_text(GTK_LABEL(temp_label),OBJ_GET(temp_label,"f_label"));

	return TRUE;
}


EXPORT gboolean table_gen_process_and_dl(GtkWidget *widget, gpointer data)
{
#define CTS 0
#define MAT 1
	gboolean celsius;
	gdouble t[3];
	gdouble c11,c12,c13;
	gdouble c21,c22,c23;
	gdouble c31,c32,c33;
	gdouble temp1,temp2,temp3;
	gdouble A,B,C;
	gdouble res1,res2,res3;
	gdouble bias;
	guint8 tabletype = 0;
	gint16 table[1024];
	gint i = 0;
	gint adcCount = 0;
	gdouble res = 0;
	gdouble temp = 0;
	gshort tt = 0;
	gint bins;
	gchar * filename = NULL;
	time_t tim;
	FILE *f = NULL;
	extern Firmware_Details *firmware;

	/* OK Button pressed,  we need to validate all input and calculate
	 * and build the tables and send to the ECU
	 */

	tabletype = gtk_combo_box_get_active(GTK_COMBO_BOX(lookup_widget("thermister_sensor_combo")));
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget("thermister_celsius_radiobutton"))))
		celsius = TRUE;
	else
		celsius = FALSE;

	bias = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("bias_entry"))),NULL);
	t[0] = temp1 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("temp1_entry"))),NULL);
	t[1] = temp2 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("temp2_entry"))),NULL);
	t[2] = temp3 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("temp3_entry"))),NULL);
	res1 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("resistance1_entry"))),NULL);
	res2 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("resistance2_entry"))),NULL);
	res3 = g_ascii_strtod(gtk_entry_get_text(GTK_ENTRY(lookup_widget("resistance3_entry"))),NULL);
	/* If Not celsius,  convert to celsius, then to kelvin */
	for (i = 0; i < 3; i++) {
		if (!celsius) 
			t[i] = (t[i]-32.0)*5.0/9.0;
		t[i] += 273.15;
	}

	c11 = log(res1); c12 = pow(c11, 3.0); c13 = 1.0 / t[0];
	c21 = log(res2); c22 = pow(c21, 3.0); c23 = 1.0 / t[1];
	c31 = log(res3); c32 = pow(c31, 3.0); c33 = 1.0 / t[2];

	C = ((c23-c13) - (c33-c13)*(c21-c11)/(c31-c11)) / ((c22-c12) - (c32-c12)*(c21-c11)/(c31-c11));
	B = (c33-c13 - C*(c32-c12)) / (c31-c11);
	A = c13 - B*c11 - C*c12;

#define k2f(t)	((t*9.0/5.0) - 459.67)
#define Tk(R)  (1.0/(A+B*log(R)+C*pow(log(R), 3)))
#define Tf(R)	(k2f(Tk(R)))
#define Tu(f,celsius) (celsius ? ((f-32.0)*5.0/9.0) : f)

	if (firmware->capabilities & PIS)
	{
		bins = 256;
		bias = 348;
	}
	else
		bins = 1024;
	time(&tim);

	filename = g_build_filename(HOME(), "thermal.log",NULL);
	f = fopen(filename, "w");
	g_free(filename);

	fprintf(f, "//------------------------------------------------------------------------------\n");
	fprintf(f, "//--  Generated by MegaTunix %s", ctime(&tim));
	fprintf(f, "//--  This file merely records what was sent to your MS-II, and may be        --\n");
	fprintf(f, "//--  deleted at any time.                                                    --\n");
	fprintf(f, "//--                                                                          --\n");
	fprintf(f, "//--  Type = %d (%s)                                                          --\n", tabletype, tabletype==CTS?"CTS":"MAT");
	fprintf(f, "//--  Bias = %7.1f                                                          --\n", bias);
	fprintf(f, "//--                                                                          --\n");
	fprintf(f, "//--  Resistance      tInput             tComputed                            --\n");
	fprintf(f, "//--  ------------    -----------------  ---------                            --\n");
	fprintf(f, "//--  %8.1f ohm  %7.1fK (% 7.1f%c) %10.1f                            --\n", res1, t[0], temp1, celsius?'C':'F', Tu(Tf(res1), celsius));
	fprintf(f, "//--  %8.1f ohm  %7.1fK (% 7.1f%c) %10.1f                            --\n", res2, t[1], temp2, celsius?'C':'F', Tu(Tf(res2), celsius));
	fprintf(f, "//--  %8.1f ohm  %7.1fK (% 7.1f%c) %10.1f                            --\n", res3, t[2], temp3, celsius?'C':'F', Tu(Tf(res3), celsius));
	fprintf(f, "//------------------------------------------------------------------------------\n");
	fprintf(f, "\n");
	fprintf(f, "#ifndef GCC_BUILD\n");
	fprintf(f, "#pragma ROM_VAR %s_ROM\n", tabletype==CTS?"CLT":"MAT");
	fprintf(f, "#endif\n");
	fprintf(f, "const int %sfactor_table[%i] EEPROM_ATTR  = {\n", tabletype==CTS?"clt":"mat",bins);
	fprintf(f, "          //  ADC    Volts    Temp       Ohms\n");

	for (adcCount = 0; adcCount < bins; adcCount++) 
	{
		res  = bias / ((bins-1)/(double)(adcCount==0?0.01:adcCount) - 1.0);
		temp = Tf(res);
		if (!(firmware->capabilities & PIS))
		{
			if      (temp <  -40.0) temp = tabletype == CTS ? 180.0 : 70.0;
			else if (temp >  350.0) temp = tabletype == CTS ? 180.0 : 70.0;
		}
		tt = (gushort)(temp*10);
		fprintf(f, "   %5d%c // %4d %7.2f  %7.1f  %9.1f\n", tt, adcCount<bins-1?',':' ', adcCount, 5.0*adcCount/(bins-1), Tu(tt/10.0, celsius), res);
		table[adcCount] = GINT16_TO_BE((gint16)tt);
		/*table[adcCount] = tt;*/
	}
	fprintf(f, "};\n");
	fprintf(f, "#ifndef GCC_BUILD\n");
	fprintf(f, "#pragma ROM_VAR DEFAULT\n");
	fprintf(f, "#endif\n");
	fprintf(f, "//------------------------------------------------------------------------------\n");
	fclose(f);

	if (tabletype == CTS)
		table_write(firmware->clt_table_page,
			firmware->page_params[firmware->clt_table_page]->length,
		       	(guint8 *)table);
	else if (tabletype == MAT)
		table_write(firmware->mat_table_page,
			firmware->page_params[firmware->mat_table_page]->length,
		       	(guint8 *)table);
	else
		printf(_("Serious ERROR!, tabletype is not CTS or MAT\n"));

	return TRUE;
}
