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
#include <datamgmt.h>
#include <debugging.h>
#include <defines.h>
#include <firmware.h>
#include <string.h>


extern GObject *global_data;
/*!
 \brief get_ecu_data() is a func to return the data requested.
 \param canID, CAN Identifier (currently unused)
 \param page, (ecu firmware page)
 \param offset, (RAW BYTE offset)
 \param size, (size to be returned)
 */
gint get_ecu_data(gint canID, gint page, gint offset, DataSize size) 
{
	extern Firmware_Details *firmware;
	/* Sanity checking */
	if (!firmware)
		return 0;
	if (!firmware->page_params)
		return 0;
	if (!firmware->page_params[page])
		return 0;
	if ((offset < 0 ) || (offset > firmware->page_params[page]->length))
		return 0;

	return _get_sized_data(firmware->ecu_data[page],page,offset,size);
}


/*!
 \brief get_ecu_data_last() is a func to return the data requested.
 \param canID, CAN Identifier (currently unused)
 \param page, (ecu firmware page)
 \param offset, (RAW BYTE offset)
 \param size, (size to be returned)
 */
gint get_ecu_data_last(gint canID, gint page, gint offset, DataSize size) 
{
	extern Firmware_Details *firmware;
	if (!firmware)
		return 0;
	if (!firmware->page_params)
		return 0;
	if (!firmware->page_params[page])
		return 0;
	if ((offset < 0 ) || (offset > firmware->page_params[page]->length))
		return 0;
	return _get_sized_data(firmware->ecu_data_last[page],page,offset,size);
}


/*!
 \brief get_ecu_data_backup() is a func to return the data requested.
 \param canID, canIdentifier (currently unused)
 \param page ( ecu firmware page)
 \param offset (RAW BYTE offset)
 \param size (size to be returned...
 */
gint get_ecu_data_backup(gint canID, gint page, gint offset, DataSize size) 
{
	extern Firmware_Details *firmware;
	if (!firmware)
		return 0;
	if (!firmware->page_params)
		return 0;
	if (!firmware->page_params[page])
		return 0;
	if ((offset < 0 ) || (offset > firmware->page_params[page]->length))
		return 0;
	return _get_sized_data(firmware->ecu_data_backup[page],page,offset,size);
}

/*!
 \brief _get_sized_data() is a func to return the data requested.
 The data is casted to the passed type.
 \param data array of data to pull from.
 \param page ( ecu firmware page)
 \param offset (RAW BYTE offset)
 \param size (size to be returned...
 */
gint _get_sized_data(guint8 *data, gint page, gint offset, DataSize size) 
{
	/* canID unused currently */
	extern Firmware_Details *firmware;
	gint result = 0;
	guint16 u16 = 0;
	gint16 s16 = 0;
	guint32 u32 = 0;
	gint32 s32 = 0;
	assert(offset >= 0);

	if (!firmware)
		return 0;
	if (!firmware->page_params)
		return 0;
	if (!firmware->page_params[page])
		return 0;
	switch (size)
	{
		case MTX_CHAR:
		case MTX_U08:
/*			printf("8 bit, returning %i\n",(guint8)data[offset]);*/
			return (guint8)data[offset];
			break;
		case MTX_S08:
			return (gint8)data[offset];
			break;
		case MTX_U16:
			u16 = ((guint8)data[offset] +((guint8)data[offset+1] << 8));
			result = GUINT16_FROM_BE(u16);
/*			printf("U16 bit, returning %i\n",result);*/
			return (guint16)result;
			break;
		case MTX_S16:
			s16 = ((guint8)data[offset] +((guint8)data[offset+1] << 8));
			result = GINT16_FROM_BE(s16);
			return (gint16)result;
/*			printf("S16 bit, returning %i\n",result);*/
			break;
		case MTX_U32:
			u32 = ((guint8)data[offset] +((guint8)data[offset+1] << 8)+((guint8)data[offset+2] << 16)+((guint8)data[offset+3] << 24));
			result = GUINT_FROM_BE(u32);
			return (guint32)result;
			break;
		case MTX_S32:
			s32 = ((guint8)data[offset] +((guint8)data[offset+1] << 8)+((guint8)data[offset+2] << 16)+((guint8)data[offset+3] << 24));
			result = GINT_FROM_BE(u32);
			return (gint32)result;
			break;
		default:
			break;
	}
	return 0;
}
 

void set_ecu_data(gint canID, gint page, gint offset, DataSize size, gint new) 
{
	extern Firmware_Details *firmware;
	_set_sized_data(firmware->ecu_data[page],offset,size,new);
}


void _set_sized_data(guint8 *data, gint offset, DataSize size, gint new) 
{
	/* canID unused currently */
	guint16 u16 = 0;
	gint16 s16 = 0;
	guint32 u32 = 0;
	gint32 s32 = 0;

	if (!data)
		return;
	switch (size)
	{
		case MTX_CHAR:
		case MTX_U08:
			data[offset] = (guint8)new;
			break;
		case MTX_S08:
			data[offset] = (gint8)new;
			break;
		case MTX_U16:
			u16 = GUINT16_TO_BE((guint16)new);
			data[offset] = (guint8)u16;
			data[offset+1] = (guint8)((guint16)u16 >> 8);
			break;
		case MTX_S16:
			s16 = GINT16_TO_BE((gint16)new);
			data[offset] = (guint8)s16;
			data[offset+1] = (guint8)((gint16)s16 >> 8);
			break;
		case MTX_U32:
			u32 = GUINT32_TO_BE((guint32)new);
			data[offset] = (guint8)u32;
			data[offset+1] = (guint8)((guint32)u32 >> 8);
			data[offset+2] = (guint8)((guint32)u32 >> 16);
			data[offset+3] = (guint8)((guint32)u32 >> 24);
			break;
		case MTX_S32:
			s32 = GINT32_TO_BE((gint32)new);
			data[offset] = (guint8)s32;
			data[offset+1] = (guint8)((gint32)s32 >> 8);
			data[offset+2] = (guint8)((gint32)s32 >> 16);
			data[offset+3] = (guint8)((gint32)s32 >> 24);
			break;
		default:
			printf(_("ERROR! attempted set of data with NO SIZE defined\n"));
	}
}
 
void store_new_block(gint canID, gint page, gint offset, void * buf, gint count)
{
	extern Firmware_Details *firmware;
	guint8 ** ecu_data = NULL;

	if (!firmware)
		return;
	if (!firmware->ecu_data)
		return;
	if (!firmware->ecu_data[page])
		return;

	ecu_data = firmware->ecu_data;
	memcpy (ecu_data[page]+offset,buf,count);
}


void backup_current_data(gint canID, gint page)
{
	extern Firmware_Details *firmware;
	guint8 ** ecu_data = NULL;
	guint8 ** ecu_data_last = NULL;

	if (!firmware)
		return;
	if (!firmware->ecu_data)
		return;
	if (!firmware->ecu_data_last)
		return;
	if (!firmware->ecu_data[page])
		return;
	if (!firmware->ecu_data_last[page])
		return;

	ecu_data = firmware->ecu_data;
	ecu_data_last = firmware->ecu_data_last;
	memcpy (ecu_data_last[page],ecu_data[page],firmware->page_params[page]->length);
}


/*!
 \brief find_mtx_page() is a func to return the data requested.
 \param tableID, Table Identified (physical ecu page)
 \param mtx_page, The symbolic page mtx uses to get around the nonlinear
 nature of the page layout in certain MS firmwares
 \returns true on success, false on failure
 */
gboolean find_mtx_page(gint tableID,gint *mtx_page)
{
	extern Firmware_Details *firmware;
	gint i = 0;

	if (!firmware)
		return FALSE;
	if (!firmware->page_params)
		return FALSE;

	for (i=0;i<firmware->total_pages;i++)
	{
		if (!firmware->page_params[i])
			return FALSE;
		if (firmware->page_params[i]->phys_ecu_page == tableID)
		{
			*mtx_page = i;
			return TRUE;
		}
	}
	return FALSE;
}
