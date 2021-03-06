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

#include <args.h>
#include <config.h>
#include <comms.h>
#include <conversions.h>
#include <dataio.h>
#include <datamgmt.h>
#include <defines.h>
#include <debugging.h>
#include <enums.h>
#include <firmware.h>
#include <init.h>
#include <listmgmt.h>
#include <mode_select.h>
#include <mtxsocket.h>
#include <notifications.h>
#include <runtime_gui.h>
#include <runtime_text.h>
#include <rtv_processor.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <timeout_handlers.h>
#include <widgetmgmt.h>


extern gint dbg_lvl;
extern GObject *global_data;

EXPORT void start_statuscounts_pf(void)
{
	start_tickler(SCOUNTS_TICKLER);
}


EXPORT void enable_reboot_button_pf(void)
{
	gdk_threads_enter();
	gtk_widget_set_sensitive(lookup_widget("error_status_reboot_button"),TRUE);
	gdk_threads_leave();
}

EXPORT void startup_tcpip_sockets_pf(void)
{
	extern gboolean interrogated;
        extern volatile gboolean offline;
	CmdLineArgs *args = NULL;

	args = OBJ_GET(global_data,"args");
	if (args->network_mode)
		return;
	if ((interrogated) && (!offline))
		open_tcpip_sockets();
}

EXPORT void spawn_read_ve_const_pf(void)
{
	extern Firmware_Details *firmware;
	if (!firmware)
		return;

	gdk_threads_enter();
	set_title(g_strdup(_("Queuing read of all ECU data...")));
	gdk_threads_leave();
	io_cmd(firmware->get_all_command,NULL);
}


EXPORT gboolean burn_all_helper(void *data, XmlCmdType type)
{
	extern Firmware_Details *firmware;
	extern volatile gboolean offline;
	OutputData *output = NULL;
	Command *command = NULL;
	extern volatile gint last_page;
	gint i = 0;

	if (last_page == -1)
	{
		command = (Command *)data;
		io_cmd(NULL,command->post_functions);
		return TRUE;
	}
	/*printf("burn all helper\n");*/
	if ((type != MS2) && (type != MS1))
		return FALSE;
	if (!offline)
	{
		/* MS2 extra is slightly different as it's paged like MS1 */
		if (((firmware->capabilities & MS2_E) || (firmware->capabilities & MS1) || (firmware->capabilities & MSNS_E)))
		{
	/*		printf("paged burn\n");*/
			output = initialize_outputdata();
			OBJ_SET(output->object,"page",GINT_TO_POINTER(last_page));
			OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[last_page]->phys_ecu_page));
			OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
			OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
			io_cmd(firmware->burn_command,output);
		}
		else if ((firmware->capabilities & MS2) && (!(firmware->capabilities & MS2_E)))
		{
			/* MS2 std allows all pages to be in ram at will*/
			for (i=0;i<firmware->total_pages;i++)
			{
				if (!firmware->page_params[i]->dl_by_default)
					continue;
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(i));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[i]->phys_ecu_page));
				OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->burn_command,output);
			}
		}
	}
	command = (Command *)data;
	io_cmd(NULL,command->post_functions);
	return TRUE;
}

EXPORT gboolean read_ve_const(void *data, XmlCmdType type)
{
	extern Firmware_Details *firmware;
	extern volatile gboolean offline;
	extern volatile gboolean outstanding_data;
	extern volatile gint last_page;
	OutputData *output = NULL;
	Command *command = NULL;
	gint i = 0;

	switch (type)
	{
		case MS1_VECONST:

			if (!offline)
			{
				g_list_foreach(get_list("get_data_buttons"),set_widget_sensitive,GINT_TO_POINTER(FALSE));
				if (outstanding_data)
					queue_burn_ecu_flash(last_page);
				for (i=0;i<firmware->total_pages;i++)
				{
					if (!firmware->page_params[i]->dl_by_default)
						continue;
					queue_ms1_page_change(i);
					output = initialize_outputdata();
					OBJ_SET(output->object,"page",GINT_TO_POINTER(i));
					OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[i]->phys_ecu_page));
					OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
					io_cmd(firmware->ve_command,output);
				}
			}
			command = (Command *)data;
			io_cmd(NULL,command->post_functions);
			break;
		case MS2_VECONST:
			if (!offline)
			{
				g_list_foreach(get_list("get_data_buttons"),set_widget_sensitive,GINT_TO_POINTER(FALSE));
				if ((firmware->capabilities & MS2_E) && (outstanding_data))
					queue_burn_ecu_flash(last_page);
				for (i=0;i<firmware->total_pages;i++)
				{
					if (!firmware->page_params[i]->dl_by_default)
						continue;
					output = initialize_outputdata();
					OBJ_SET(output->object,"page",GINT_TO_POINTER(i));
					OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[i]->phys_ecu_page));
					OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
					OBJ_SET(output->object,"offset", GINT_TO_POINTER(0));
					OBJ_SET(output->object,"num_bytes", GINT_TO_POINTER(firmware->page_params[i]->length));
					OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
					io_cmd(firmware->ve_command,output);
				}
			}
			command = (Command *)data;
			io_cmd(NULL,command->post_functions);
			break;
		case MS2_E_COMPOSITEMON:
			if (!offline)
			{
				if ((firmware->capabilities & MS2_E) && (outstanding_data))
					queue_burn_ecu_flash(last_page);
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(firmware->compositemon_page));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[firmware->compositemon_page]->phys_ecu_page));
				OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
				OBJ_SET(output->object,"offset", GINT_TO_POINTER(0));
				OBJ_SET(output->object,"num_bytes", GINT_TO_POINTER(firmware->page_params[firmware->compositemon_page]->length));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->ve_command,output);
			}
			command = (Command *)data;
			io_cmd(NULL,command->post_functions);
			break;
		case MS2_E_TRIGMON:
			if (!offline)
			{
				if ((firmware->capabilities & MS2_E) && (outstanding_data))
					queue_burn_ecu_flash(last_page);
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(firmware->trigmon_page));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[firmware->trigmon_page]->phys_ecu_page));
				OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
				OBJ_SET(output->object,"offset", GINT_TO_POINTER(0));
				OBJ_SET(output->object,"num_bytes", GINT_TO_POINTER(firmware->page_params[firmware->trigmon_page]->length));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->ve_command,output);
			}
			command = (Command *)data;
			io_cmd(NULL,command->post_functions);
			break;
		case MS2_E_TOOTHMON:
			if (!offline)
			{
				if ((firmware->capabilities & MS2_E) && (outstanding_data))
					queue_burn_ecu_flash(last_page);
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(firmware->toothmon_page));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[firmware->toothmon_page]->phys_ecu_page));
				OBJ_SET(output->object,"canID",GINT_TO_POINTER(firmware->canID));
				OBJ_SET(output->object,"offset", GINT_TO_POINTER(0));
				OBJ_SET(output->object,"num_bytes", GINT_TO_POINTER(firmware->page_params[firmware->toothmon_page]->length));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->ve_command,output);
			}
			command = (Command *)data;
			io_cmd(NULL,command->post_functions);
			break;
		case MS1_E_TRIGMON:
			if (!offline)
			{
				if (outstanding_data)
					queue_burn_ecu_flash(last_page);
				queue_ms1_page_change(firmware->trigmon_page);
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(firmware->trigmon_page));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[firmware->trigmon_page]->phys_ecu_page));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->ve_command,output);
				command = (Command *)data;
				io_cmd(NULL,command->post_functions);
			}
			break;
		case MS1_E_TOOTHMON:
			if (!offline)
			{
				if (outstanding_data)
					queue_burn_ecu_flash(last_page);
				queue_ms1_page_change(firmware->toothmon_page);
				output = initialize_outputdata();
				OBJ_SET(output->object,"page",GINT_TO_POINTER(firmware->toothmon_page));
				OBJ_SET(output->object,"phys_ecu_page",GINT_TO_POINTER(firmware->page_params[firmware->toothmon_page]->phys_ecu_page));
				OBJ_SET(output->object,"mode", GINT_TO_POINTER(MTX_CMD_WRITE));
				io_cmd(firmware->ve_command,output);
				command = (Command *)data;
				io_cmd(NULL,command->post_functions);
			}
			break;
		default:
			break;
	}
	return TRUE;
}


EXPORT void enable_get_data_buttons_pf(void)
{
	gdk_threads_enter();
	g_list_foreach(get_list("get_data_buttons"),set_widget_sensitive,GINT_TO_POINTER(TRUE));
	gdk_threads_leave();
}


EXPORT void enable_ttm_buttons_pf(void)
{
	gdk_threads_enter();
	g_list_foreach(get_list("ttm_buttons"),set_widget_sensitive,GINT_TO_POINTER(TRUE));
	gdk_threads_leave();
}


EXPORT void conditional_start_rtv_tickler_pf(void)
{
	static gboolean just_starting = TRUE;

	if (just_starting)
	{
		start_tickler(RTV_TICKLER);
		just_starting = FALSE;
	}
}


EXPORT void set_store_black_pf(void)
{
	gint j = 0;
	extern Firmware_Details *firmware;

	gdk_threads_enter();
	set_group_color(BLACK,"burners");
	for (j=0;j<firmware->total_tables;j++)
		set_reqfuel_color(BLACK,j);
	gdk_threads_leave();
}

EXPORT void enable_3d_buttons_pf(void)
{
	gdk_threads_enter();
	g_list_foreach(get_list("3d_buttons"),set_widget_sensitive,GINT_TO_POINTER(TRUE));
	gdk_threads_leave();
}


EXPORT void disable_burner_buttons_pf(void)
{
	gdk_threads_enter();
	g_list_foreach(get_list("burners"),set_widget_sensitive,GINT_TO_POINTER(FALSE));
	gdk_threads_leave();
}

EXPORT void reset_temps_pf(void)
{
	gdk_threads_enter();
	set_title(g_strdup(_("Adjusting for local Temp units...")));
	reset_temps(OBJ_GET(global_data,"temp_units"));
	gdk_threads_leave();
}

EXPORT void ready_msg_pf(void)
{
	gdk_threads_enter();
	set_title(g_strdup(_("Ready...")));
	gdk_threads_leave();
}

EXPORT void simple_read_pf(void * data, XmlCmdType type)
{
	Io_Message *message  = NULL;
	OutputData *output  = NULL;
	gint count = 0;
	gchar *tmpbuf = NULL;
	gint page = -1;
	gint canID = -1;
	guint16 curcount = 0;
	static guint16 lastcount = 0;
	extern Firmware_Details *firmware;
	extern gint ms_ve_goodread_count;
	extern gint ms_reset_count;
	extern gint ms_goodread_count;
	guint8 *ptr8 = NULL;
	guint16 *ptr16 = NULL;
	static gboolean just_starting = TRUE;
	extern gboolean forced_update;
	extern gboolean force_page_change;
	extern volatile gboolean offline;


	message = (Io_Message *)data;
	output = (OutputData *)message->payload;

	switch (type)
	{
		case WRITE_VERIFY:
			printf(_("MS2_WRITE_VERIFY not written yet\n"));
			break;
		case MISMATCH_COUNT:
			printf(_("MS2_MISMATCH_COUNT not written yet\n"));
			break;
		case MS1_CLOCK:
			printf(_("MS1_CLOCK not written yet\n"));
			break;
		case MS2_CLOCK:
			printf(_("MS2_CLOCK not written yet\n"));
			break;
		case NUM_REV:
			if (offline)
				break;
			count = read_data(-1,&message->recv_buf,FALSE);
			ptr8 = (guchar *)message->recv_buf;
			firmware->ecu_revision=(gint)ptr8[0];
			if (count > 0)
				thread_update_widget("ecu_revision_entry",MTX_ENTRY,g_strdup_printf("%.1f",((gint)ptr8[0]/10.0)));
			else
				thread_update_widget("ecu_revision_entry",MTX_ENTRY,g_strdup(""));
			break;
		case TEXT_REV:
			if (offline)
				break;
			count = read_data(-1,&message->recv_buf,FALSE);
			if (count > 0)
			{
				thread_update_widget("text_version_entry",MTX_ENTRY,g_strndup(message->recv_buf,count));
				 firmware->txt_rev_len = count;
				firmware->text_revision = g_strndup(message->recv_buf,count);
			}
			break;
		case SIGNATURE:
			if (offline)
				break;
			 count = read_data(-1,&message->recv_buf,FALSE);
                         if (count > 0)
			 {
				 thread_update_widget("ecu_signature_entry",MTX_ENTRY,g_strndup(message->recv_buf,count));
				 firmware->signature_len = count;
				 firmware->actual_signature = g_strndup(message->recv_buf,count);
			 }
			break;
		case MS1_VECONST:
		case MS2_VECONST:
			page = (GINT)OBJ_GET(output->object,"page");
			canID = (GINT)OBJ_GET(output->object,"canID");
			count = read_data(firmware->page_params[page]->length,&message->recv_buf,TRUE);
			if (count != firmware->page_params[page]->length)
				break;
			store_new_block(canID,page,0,
					message->recv_buf,
					firmware->page_params[page]->length);
			backup_current_data(canID,page);
			ms_ve_goodread_count++;
			break;
		case MS1_RT_VARS:
			count = read_data(firmware->rtvars_size,&message->recv_buf,TRUE);
			if (count != firmware->rtvars_size)
				break;
			ptr8 = (guchar *)message->recv_buf;
			/* Test for MS reset */
			if (just_starting)
			{
				lastcount = ptr8[0];
				just_starting = FALSE;
			}
			/* Check for clock jump from the MS, a 
			 * jump in time from the MS clock indicates 
			 * a reset due to power and/or noise.
			 */
			if ((lastcount - ptr8[0] > 1) && \
					(lastcount - ptr8[0] != 255))
			{
				ms_reset_count++;
				printf(_("MS1 Reset detected!, lastcount %i, current %i\n"),lastcount,ptr8[0]);
				gdk_threads_enter();
				gdk_beep();
				gdk_threads_leave();
			}
			else
				ms_goodread_count++;
			lastcount = ptr8[0];
			/* Feed raw buffer over to post_process()
			 * as a void * and pass it a pointer to the new
			 * area for the parsed data...
			 */
			process_rt_vars((void *)message->recv_buf);
			break;
		case MS2_RT_VARS:
			page = (GINT)OBJ_GET(output->object,"page");
			canID = (GINT)OBJ_GET(output->object,"canID");
			count = read_data(firmware->rtvars_size,&message->recv_buf,TRUE);
			if (count != firmware->rtvars_size)
				break;
			store_new_block(canID,page,0,
					message->recv_buf,
					firmware->page_params[page]->length);
			backup_current_data(canID,page);
			ptr16 = (guint16 *)message->recv_buf;
			/* Test for MS reset */
			if (just_starting)
			{
				lastcount = GUINT16_TO_BE(ptr16[0]);
				curcount = GUINT16_TO_BE(ptr16[0]);
				just_starting = FALSE;
			}
			else
				curcount = GUINT16_TO_BE(ptr16[0]);
			/* Check for clock jump from the MS, a 
			 * jump in time from the MS clock indicates 
			 * a reset due to power and/or noise.
			 */
			if ((lastcount - curcount > 1) && \
					(lastcount - curcount != 65535))
			{
				ms_reset_count++;
				printf(_("MS2 rtvars reset detected, lastcount is %i, current %i"),lastcount,curcount);
				gdk_threads_enter();
				gdk_beep();
				gdk_threads_leave();
			}
			else
				ms_goodread_count++;
			lastcount = curcount;
			/* Feed raw buffer over to post_process()
			 * as a void * and pass it a pointer to the new
			 * area for the parsed data...
			 */
			process_rt_vars((void *)message->recv_buf);
			break;
		case MS2_BOOTLOADER:
			printf(_("MS2_BOOTLOADER not written yet\n"));
			break;
		case MS1_GETERROR:
			forced_update = TRUE;
			force_page_change = TRUE;
			count = read_data(-1,&message->recv_buf,FALSE);
			if (count <= 10)
			{
				thread_update_logbar("error_status_view",NULL,g_strdup(_("No ECU Errors were reported....\n")),FALSE,FALSE);
				break;
			}
			if (g_utf8_validate(((gchar *)message->recv_buf)+1,count-1,NULL))
			{
				thread_update_logbar("error_status_view",NULL,g_strndup(((gchar *)message->recv_buf+7)+1,count-8),FALSE,FALSE);
				if (dbg_lvl & (IO_PROCESS|SERIAL_RD))
				{
					tmpbuf = g_strndup(((gchar *)message->recv_buf)+1,count-1);
					dbg_func(IO_PROCESS|SERIAL_RD,g_strdup_printf(__FILE__"\tECU  ERROR string: \"%s\"\n",tmpbuf));
					g_free(tmpbuf);
				}

			}
			else
				thread_update_logbar("error_status_view",NULL,g_strdup("The data came back as gibberish, please try again...\n"),FALSE,FALSE);
			break;
		default:
			break;
	}
}

/*!
 \brief post_burn_pf() handles post burn ecu data mgmt
 */
EXPORT void post_burn_pf()
{
	gint i = 0;
	extern Firmware_Details * firmware;

	/* sync temp buffer with current burned settings */
	for (i=0;i<firmware->total_pages;i++)
	{
		if (!firmware->page_params[i]->dl_by_default)
			continue;
		backup_current_data(firmware->canID,i);
	}

	dbg_func(SERIAL_WR,g_strdup(__FILE__": post_burn_pf()\n\tBurn to Flash Completed\n"));

	return;
}


EXPORT void post_single_burn_pf(void *data)
{
	Io_Message *message = (Io_Message *)data;
	OutputData *output = (OutputData *)message->payload;
	extern Firmware_Details * firmware;
	gint page = (GINT)OBJ_GET(output->object,"page");

	/* sync temp buffer with current burned settings */
	if (!firmware->page_params[page]->dl_by_default)
		return;
	backup_current_data(firmware->canID,page);

	dbg_func(SERIAL_WR,g_strdup(__FILE__": post_single_burn_pf()\n\tBurn to Flash Completed\n"));

	return;
}


EXPORT void startup_default_timeouts_pf()
{
	gint source = 0;
	gint rate = 0;

	gdk_threads_enter();
	set_title(g_strdup(_("Starting up data renderers...")));
	gdk_threads_leave();
	rate = (GINT)OBJ_GET(global_data,"rtslider_fps");
	source = g_timeout_add((gint)(1000.0/(gfloat)rate),(GtkFunction)update_rtsliders,NULL);
	OBJ_SET(global_data,"rtslider_id", GINT_TO_POINTER(source));

	rate = (GINT)OBJ_GET(global_data,"rttext_fps");
	source = g_timeout_add((gint)(1000.0/(gfloat)rate),(GtkFunction)update_rttext,NULL);
	OBJ_SET(global_data,"rttext_id", GINT_TO_POINTER(source));

	rate = (GINT)OBJ_GET(global_data,"dashboard_fps");
	source = g_timeout_add((gint)(1000.0/(gfloat)rate),(GtkFunction)update_dashboards,NULL);
	OBJ_SET(global_data,"dashboard_id", GINT_TO_POINTER(source));

	rate = (GINT)OBJ_GET(global_data,"ve3d_fps");
	source = g_timeout_add((gint)(1000.0/(gfloat)rate),(GtkFunction)update_ve3ds,NULL);
	OBJ_SET(global_data,"ve3d_id", GINT_TO_POINTER(source));
}

