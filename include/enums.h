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

#ifndef __ENUMS_H__
#define __ENUMS_H__

/* Serial data comes in and handled by the following possibilities.
 * dataio.c uses these to determine which course of action to take...
 */
typedef enum
{
	REALTIME_VARS,
	VE_AND_CONSTANTS_1,
	VE_AND_CONSTANTS_2,
	IGNITION_VARS,
	RAW_MEMORY
}InputData;

/* Regular Buttons */
typedef enum
{
	START_REALTIME,
	STOP_REALTIME,
	READ_VE_CONST,
	BURN_MS_FLASH,
	SELECT_DLOG_EXP,
	SELECT_DLOG_IMP,
	CLOSE_LOGFILE,
	START_DATALOGGING,
	STOP_DATALOGGING,
	EXPORT_VETABLE,
	IMPORT_VETABLE,
	REVERT_TO_BACKUP,
	BACKUP_ALL,
	RESTORE_ALL,
	SELECT_PARAMS,
	REQD_FUEL_POPUP,
}StdButton;

/* Toggle/Radio buttons */
typedef enum
{
	TOOLTIPS_STATE,
        FAHRENHEIT,
        CELSIUS,
	COMMA,
	TAB,
	SPACE,
	REALTIME_VIEW,
	PLAYBACK_VIEW
}ToggleButton;

typedef enum
{
	MT_CLASSIC_LOG,
	MT_FULL_LOG,
	CUSTOM_LOG
}LoggingMode;
	
/* spinbuttons... */
typedef enum
{
	REQ_FUEL_DISP,
	REQ_FUEL_CYLS,
	REQ_FUEL_RATED_INJ_FLOW,
	REQ_FUEL_RATED_PRESSURE,
	REQ_FUEL_ACTUAL_PRESSURE,
	REQ_FUEL_AFR,
	REQ_FUEL_1,
	REQ_FUEL_2,
	SER_POLL_TIMEO,
	SER_INTERVAL_DELAY,
	SET_SER_PORT,
	NUM_SQUIRTS_1,
	NUM_SQUIRTS_2,
	NUM_CYLINDERS_1,
	NUM_CYLINDERS_2,
	NUM_INJECTORS_1,
	NUM_INJECTORS_2,
	GENERIC
}SpinButton;

/* Conversions for download, converse on upload.. */
typedef enum
{
	ADD,
	SUB,
	MULT,
	DIV,
	NOTHING
}Conversions;

/* Runtime Status flags */
typedef enum 
{       
	CONNECTED, 
        CRANKING, 
        RUNNING, 
        WARMUP, 
        AS_ENRICH, 
        ACCEL, 
        DECEL
}RuntimeStatus;

typedef enum
{
	VE_EXPORT = 10,
	VE_IMPORT,
	DATALOG_EXPORT,
	DATALOG_IMPORT,
	FULL_BACKUP,
	FULL_RESTORE
}FileIoType;


typedef enum
{	
	RED,
	BLACK
}GuiState;

typedef enum
{
	HEADER,
	PAGE,
	RANGE,
	TABLE
}ImportParserFunc;

typedef enum
{
	EVEME,
	USER_REV,
	USER_COMMENT,
	DATE,
	TIME,
	RPM_RANGE,
	LOAD_RANGE,
	NONE
}ImportParserArg;

typedef enum
{
	FONT,
	TRACE,
	GRATICULE
}GcType;

#endif
