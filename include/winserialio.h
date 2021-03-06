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

#ifndef __WINSERIALIO_H__
#define __WINSERIALIO_H__

typedef enum
{
	INBOUND=0x2E0,
	OUTBOUND,
	BOTH
}FlushDirection;


/* Prototypes */
void win32_setup_serial_params(int, int);
void win32_toggle_serial_control_lines(void);
void win32_flush_serial(int, FlushDirection);
/* Prototypes */

#endif
