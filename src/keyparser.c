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
 * No warranty is made or implied. You use this program at your own risk.
 */

#include <assert.h>
#include <config.h>
#include <configfile.h>
#include <defines.h>
#include <debugging.h>
#include <enums.h>
#include <keyparser.h>
#include <stringmatch.h>


extern GObject *global_data;
/*!
 \brief parse_keys() splits up a string list into a vector and returns it
 and the number of keys
 \param string (gchar *) input string
 \param count (gint *) reference to dest to place number of keys
 \param delimiter (gchar *) char to split the string with
 \returns a string vector of the original string split up with the delimiter
 */
gchar ** parse_keys(gchar * string, gint * count, gchar *delimiter)
{
	gchar **result = NULL;	
	assert(string);
	if (!string)
	{
		dbg_func(KEYPARSER|CRITICAL,g_strdup(__FILE__": parse_keys()\n\t String passed was NULL\n"));
		*count = 0;
		return NULL;
	}
	result = g_strsplit(string,delimiter,0);
	*count = g_strv_length(result);
	return result;
}

/*!
 \brief parse_keytypes() splits up a string list and converts the individual 
 values into enumerations
 \param string (gchar *) input string
 \param count (gint *) reference to dest to place number of keys
 \param delimiter (gchar *) char to split the string with
 \returns a dynamic integer array of the keystypes (enums)
 */
gint * parse_keytypes(gchar * string, gint * count, gchar *delimiter)
{
	gchar **vector = NULL;	
	gint *keytypes = NULL;
	gint i = 0;
	gint ct = 0;

	assert(string);
	if (!string)
	{
		dbg_func(KEYPARSER|CRITICAL,g_strdup(__FILE__": parse_keytypes()\n\t String passed was NULL\n"));
		*count = 0;
		return 0;
	}
	vector = g_strsplit(string,delimiter,0);
	while (vector[ct])
		ct++;

	keytypes = (gint *)g_malloc0(ct*sizeof(gint));	
	while (vector[i])
	{
		dbg_func(KEYPARSER,g_strdup_printf(__FILE__": parse_keytypes()\n\ttrying to translate %s\n",vector[i]));
		keytypes[i] = translate_string(vector[i]);
		dbg_func(KEYPARSER,g_strdup_printf(__FILE__": parse_keytypes()\n\ttranslated value of %s is %i\n",vector[i],keytypes[i]));
		i++;
	}
	g_strfreev(vector);
	*count = i;	
	return keytypes;

}
