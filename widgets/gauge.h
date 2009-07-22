/*
 * Copyright (C) 2006 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 * and Christopher Mire (czb)
 *
 * Megasquirt gauge widget
 * 
 * 
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 * 
 * No warranty is made or implied. You use this program at your own risk.
 */

#ifndef MTX_GAUGE_FACE_H
#define MTX_GAUGE_FACE_H

#include <config.h>
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>



G_BEGIN_DECLS

#define MTX_TYPE_GAUGE_FACE		(mtx_gauge_face_get_type ())
#define MTX_GAUGE_FACE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), MTX_TYPE_GAUGE_FACE, MtxGaugeFace))
#define MTX_GAUGE_FACE_CLASS(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), MTX_GAUGE_FACE, MtxGaugeFaceClass))
#define MTX_IS_GAUGE_FACE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), MTX_TYPE_GAUGE_FACE))
#define MTX_IS_GAUGE_FACE_CLASS(obj)	(G_TYPE_CHECK_CLASS_TYPE ((obj), MTX_TYPE_GAUGE_FACE))
#define MTX_GAUGE_FACE_GET_CLASS	(G_TYPE_INSTANCE_GET_CLASS ((obj), MTX_TYPE_GAUGE_FACE, MtxGaugeFaceClass))


#define DRAG_BORDER 7
/* MtxClampType enum,  display clamping */
typedef enum
{
	CLAMP_UPPER = 0xaa,
	CLAMP_LOWER,
	CLAMP_NONE
}MtxClampType;


/* MtxDirection enum,  display clamping */
typedef enum
{
	ASCENDING = 0xbb,
	DESCENDING
}MtxDirection;


/* MtxRotType enum,  needle sweep rotation */
typedef enum
{
	MTX_ROT_CCW = 0,
	MTX_ROT_CW
}MtxRotType;


/* MtxPolyType enumeration,  for polygon support */
typedef enum
{
	MTX_CIRCLE = 0xcc,
	MTX_ARC,
	MTX_RECTANGLE,
	MTX_GENPOLY,
	NUM_POLYS
}MtxPolyType;


/*! GaugeColorIndex enum,  for indexing into the color arrays */
typedef enum  
{
	GAUGE_COL_BG = 0,
	GAUGE_COL_NEEDLE,
	GAUGE_COL_VALUE_FONT,
	GAUGE_COL_GRADIENT_BEGIN,
	GAUGE_COL_GRADIENT_END,
	GAUGE_NUM_COLORS
}GaugeColorIndex;


/* Text Block enumeration for the individual fields */
typedef enum
{
	TB_FONT_SCALE = 0,
	TB_X_POS,
	TB_Y_POS,
	TB_COLOR,
	TB_FONT,
	TB_TEXT,
	TB_NUM_FIELDS
}TbField;


/* Polygon enumeration for the individual fields */
typedef enum
{
	POLY_COLOR = 0,
	POLY_LINEWIDTH,
	POLY_LINESTYLE,
	POLY_JOINSTYLE,
	POLY_X,
	POLY_Y,
	POLY_WIDTH,
	POLY_HEIGHT,
	POLY_RADIUS,
	POLY_POINTS,
	POLY_FILLED,
	POLY_START_ANGLE,
	POLY_SWEEP_ANGLE,
	POLY_NUM_POINTS,
	POLY_NUM_FIELDS
}PolyField;


/* Tick Group enumeration for the individual fields */
typedef enum
{
	TG_FONT = 0,
	TG_TEXT,
	TG_TEXT_COLOR,
	TG_FONT_SCALE,
	TG_TEXT_INSET,
	TG_NUM_MAJ_TICKS,
	TG_MAJ_TICK_COLOR,
	TG_MAJ_TICK_INSET,
	TG_MAJ_TICK_LENGTH,
	TG_MAJ_TICK_WIDTH,
	TG_NUM_MIN_TICKS,
	TG_MIN_TICK_COLOR,
	TG_MIN_TICK_INSET,
	TG_MIN_TICK_LENGTH,
	TG_MIN_TICK_WIDTH,
	TG_START_ANGLE,
	TG_SWEEP_ANGLE,
	TG_NUM_FIELDS
}TgField;


/* Color Range enumeration for the individual fields */
typedef enum
{
	CR_LOWPOINT = 0,
	CR_HIGHPOINT,
	CR_COLOR,
	CR_LWIDTH,
	CR_INSET,
	CR_NUM_FIELDS
}CrField;

/* Alert Range enumeration for the individual fields */
typedef enum
{
	ALRT_LOWPOINT = 0,
	ALRT_HIGHPOINT,
	ALRT_COLOR,
	ALRT_LWIDTH,
	ALRT_INSET,
	ALRT_NUM_FIELDS
}AlertField;


/* General Attributes enumeration */
typedef enum
{
	DUMMY = 0,
	START_ANGLE,
	SWEEP_ANGLE,
	ROTATION,
	LBOUND,
	UBOUND,
	VALUE_FONTSCALE,
	VALUE_XPOS,
	VALUE_YPOS,
	NEEDLE_TAIL,
	NEEDLE_LENGTH,
	NEEDLE_WIDTH,
	NEEDLE_TIP_WIDTH,
	NEEDLE_TAIL_WIDTH,
	PRECISION,
	ANTIALIAS,
	TATTLETALE,
	TATTLETALE_ALPHA,
	SHOW_VALUE,
	NUM_ATTRIBUTES
}MtxGenAttr;


typedef struct _MtxGaugeFace		MtxGaugeFace;
typedef struct _MtxGaugeFaceClass	MtxGaugeFaceClass;
typedef struct _MtxColorRange		MtxColorRange;
typedef struct _MtxAlertRange		MtxAlertRange;
typedef struct _MtxTextBlock		MtxTextBlock;
typedef struct _MtxTickGroup		MtxTickGroup;
typedef struct _MtxDispatchHelper	MtxDispatchHelper;
typedef struct _MtxPoint		MtxPoint;
typedef struct _MtxPolygon		MtxPolygon;
typedef struct _MtxCircle		MtxCircle;
typedef struct _MtxArc			MtxArc;
typedef struct _MtxRectangle		MtxSquare;
typedef struct _MtxRectangle		MtxRectangle;
typedef struct _MtxGenPoly		MtxGenPoly;


struct _MtxDispatchHelper
{
	gchar * element_name;
	gpointer src;
	xmlNodePtr root_node;
	MtxGaugeFace * gauge;
};


/*! \struct _MtxColorRange
 * \brief
 * _MtxColorRange is a container struct that holds all the information needed
 * for a color range span on a gauge. Any gauge can have an arbritrary number
 * of these structs as they are stored in a dynamic array and redrawn on
 * gauge background generation
 */
struct _MtxColorRange
{
	gfloat lowpoint;	/* where the range starts from */
	gfloat highpoint; 	/* where the range ends at */
	GdkColor color;		/* The color to use */
	gfloat lwidth;		/* % of radius to determine the line width */
	gfloat inset;		/* % of radius to inset the line */
};


/*! \struct _MtxAlertRange
 * \brief
 * _MtxAlertRange is a container struct that holds all the information needed
 * for an alert range span on a gauge. Any gauge can have an arbritrary number
 * of these structs as they are stored in a dynamic array and redrawn on
 * gauge update if the gauge value is within the limits of the alert. 
 */
struct _MtxAlertRange
{
	gfloat lowpoint;	/* where the range starts from */
	gfloat highpoint; 	/* where the range ends at */
	GdkColor color;		/* The color to use */
	gfloat lwidth;		/* % of radius to determine the line width */
	gfloat inset;		/* % of radius to inset the line */
};


/*! \struct _MtxTextBlock
 * \brief
 * _MtxTextBlock is a container struct that holds information for a text
 * block to be placed somplace on a gauge.  A dynamic array holds the pointers
 * to these structs so that any number of them can be created.
 */
struct _MtxTextBlock
{
	gchar * font;
	gchar * text;
	GdkColor color;
	gfloat font_scale;
	gfloat x_pos;
	gfloat y_pos;
};

/*! \struct _MtxTickGroup
 * \brief
 * _MtxTickGroup is a container structure that holds all the info needed for
 * a group of tickmarks.  This is added to allow multiple groups of tickmarks
 * that share a common set of attributes.  There is no limit (Aside from RAM)
 * to the number of tickgroups you can have in a gauge. This allows for max
 * flexibility in gauge design
 */
struct _MtxTickGroup
{
	gchar *font;
	gchar *text;
	GdkColor text_color;
	gfloat font_scale;
	gfloat text_inset;
	gint num_maj_ticks;
	GdkColor maj_tick_color;
	gfloat maj_tick_inset;
	gfloat maj_tick_width;
	gfloat maj_tick_length;
	gint num_min_ticks;
	GdkColor min_tick_color;
	gfloat min_tick_inset;
	gfloat min_tick_width;
	gfloat min_tick_length;
	gfloat start_angle;
	gfloat sweep_angle;
};


/*! \struct _MtxPoint
 * \brief
 * _MtxPoint houses a coordinate in gauge space in floating point coords which
 * are percentages of radius to keep everything scalable
 */
struct _MtxPoint
{
	gfloat x;		/* X val as % of gauge radius */
	gfloat y;		/* Y val as % of gauge radius */
};

/*! \struct _MtxPolygon
 * \brief
 * _MtxPolygon is a container struct that holds an identifier (enum) and a 
 * void * for the the actualy polygon data (the struct will be casted in the
 * code to the right type to get to the correct data...
 */
struct _MtxPolygon
{
	MtxPolyType type;		/* Enum type */
	gboolean filled;		/* Filled or not? */
	GdkColor color;			/* Color */
	gfloat line_width;		/* % of radius, clamped at 1 pixel */
	GdkLineStyle line_style;	/* Line Style */
	GdkJoinStyle join_style;	/* Join Style */
	void *data;			/* point to datastruct */
};


/*! \struct _MtxCircle 
 * \brief
 * _MtxCircle contains the info needed to create a circle on the gauge.
 * Values are in floating point as they are percentages of gauge radius so
 * that things are fully scalable
 */
struct _MtxCircle
{
	/* All float values excpet for angles are percentages of radius,
	 * with respect to the center of the gauge
	 * so an x value of 0 is the CENTER, a value of -1 is the left border
	 * and +1 is the right border.
	 */
	gfloat x;		/* X center */
	gfloat y;		/* Y center */
	gfloat radius;		/* radius of circle as a % fo gauge radius */
};


/*! \struct _MtxArc
 * \brief
 * _MtxArc contains the info needed to create an arc on the gauge.
 * Values are in floating point as they are percentages of gauge radius so
 * that things are fully scalable
 */
struct _MtxArc
{
	/* All float values excpet for angles are percentages of radius,
	 * with respect to the center of the gauge
	 * so an x value of 0 is the CENTER, a value of -1 is the left border
	 * and +1 is the right border.
	 */
	gfloat x;		/* Left edge of bounding rectangle */
	gfloat y;		/* Right edge of bounding rectangle */
	gfloat width;		/* width of bounding rectangle */
	gfloat height;		/* height of bounding rectangle */
	gfloat start_angle;	/* 0 deg is "3 O'Clock" CCW rotation */
	gfloat sweep_angle;	/* Angle relative to start_angle */
};


/*! \struct _MtxRectangle _MtxSquare
 * \brief
 * _MtxRectangle contains the info needed to create a rect on the gauge.
 * Values are in floating point as they are percentages of gauge radius so
 * that things are fully scalable
 */
struct _MtxRectangle
{
	/* All float values excpet for angles are percentages of radius,
	 * with respect to the center of the gauge
	 * so an x value of 0 is the CENTER, a value of -1 is the left border
	 * and +1 is the right border.
	 */
	gfloat x;		/* Top left edge x coord (% of rad) */
	gfloat y;		/* Top left edge y coord (% of rad) */
	gfloat width;		/* Width */
	gfloat height;		/* Height */
};

/*! \struct _MtxGenPoly
 * \brief
 * _MtxGenPoly is a container structure for generic polygons that can't be
 * described by the generic choices above. It allows for any number of points
 * with one fixed color, in a filled or unfilled state the  ui for these though
 * needs improvement.
 */
struct _MtxGenPoly
{
	gint num_points;	/* Number of points */
	MtxPoint *points;	/* Dynamic array of points */
};

struct _MtxGaugeFace
{	/* public data */
	GtkDrawingArea parent;
};

struct _MtxGaugeFaceClass
{
	GtkDrawingAreaClass parent_class;
};

GType mtx_gauge_face_get_type (void);
void generate_gauge_background(MtxGaugeFace *);
void update_gauge_position (MtxGaugeFace *);
GtkWidget* mtx_gauge_face_new ();

/* Gauge General Attributes */
void mtx_gauge_face_set_attribute(MtxGaugeFace *gauge, MtxGenAttr field, gfloat value);
gboolean mtx_gauge_face_get_attribute(MtxGaugeFace *gauge, MtxGenAttr field, gfloat * value);

/* Get/Set value */
void mtx_gauge_face_set_value (MtxGaugeFace *gauge, gfloat value);
float mtx_gauge_face_get_value (MtxGaugeFace *gauge);

/* Value Font */
void mtx_gauge_face_set_value_font (MtxGaugeFace *gauge, gchar *);
gchar * mtx_gauge_face_get_value_font (MtxGaugeFace *gauge);

/* Color Ranges */
GArray * mtx_gauge_face_get_color_ranges(MtxGaugeFace *gauge);
void mtx_gauge_face_alter_color_range(MtxGaugeFace *gauge, gint index, CrField field, void * value);
gint mtx_gauge_face_set_color_range_struct(MtxGaugeFace *gauge, MtxColorRange *);
void mtx_gauge_face_remove_color_range(MtxGaugeFace *gauge, gint index);
void mtx_gauge_face_remove_all_color_ranges(MtxGaugeFace *gauge);

/* Alert Ranges */
GArray * mtx_gauge_face_get_alert_ranges(MtxGaugeFace *gauge);
void mtx_gauge_face_alter_alert_range(MtxGaugeFace *gauge, gint index, AlertField field, void * value);
gint mtx_gauge_face_set_alert_range_struct(MtxGaugeFace *gauge, MtxAlertRange *);
void mtx_gauge_face_remove_alert_range(MtxGaugeFace *gauge, gint index);
void mtx_gauge_face_remove_all_alert_ranges(MtxGaugeFace *gauge);

/* Text Blocks */
GArray * mtx_gauge_face_get_text_blocks(MtxGaugeFace *gauge);
void mtx_gauge_face_alter_text_block(MtxGaugeFace *gauge, gint index,TbField field, void * value);
gint mtx_gauge_face_set_text_block_struct(MtxGaugeFace *gauge, MtxTextBlock *);
void mtx_gauge_face_remove_text_block(MtxGaugeFace *gauge, gint index);
void mtx_gauge_face_remove_all_text_blocks(MtxGaugeFace *gauge);

/* Tick Groups */
GArray * mtx_gauge_face_get_tick_groups(MtxGaugeFace *gauge);
void mtx_gauge_face_alter_tick_group(MtxGaugeFace *gauge, gint index,TgField field, void * value);
gint mtx_gauge_face_set_tick_group_struct(MtxGaugeFace *gauge, MtxTickGroup *);
void mtx_gauge_face_remove_tick_group(MtxGaugeFace *gauge, gint index);
void mtx_gauge_face_remove_all_tick_groups(MtxGaugeFace *gauge);

/* Polygons */
GArray * mtx_gauge_face_get_polygons(MtxGaugeFace *gauge);
void mtx_gauge_face_alter_polygon(MtxGaugeFace *gauge, gint index, PolyField field, void * value);
gint mtx_gauge_face_set_polygon_struct(MtxGaugeFace *gauge, MtxPolygon *);
void mtx_gauge_face_remove_polygon(MtxGaugeFace *gauge, gint index);
void mtx_gauge_face_remove_all_polygons(MtxGaugeFace *gauge);

/* Colors */
void mtx_gauge_face_set_color (MtxGaugeFace *gauge, GaugeColorIndex index, GdkColor color);
GdkColor *mtx_gauge_face_get_color (MtxGaugeFace *gauge, GaugeColorIndex index);

/* XML */
void mtx_gauge_face_import_xml(MtxGaugeFace *, gchar *);
void mtx_gauge_face_export_xml(MtxGaugeFace *, gchar *);
gchar * mtx_gauge_face_get_xml_filename(MtxGaugeFace *gauge);

/* Misc */
void mtx_gauge_face_set_show_drag_border(MtxGaugeFace *, gboolean);
gboolean mtx_gauge_face_get_show_drag_border(MtxGaugeFace *);
void mtx_gauge_face_redraw_canvas (MtxGaugeFace *);

/* Tattletale */
float mtx_gauge_face_get_peak (MtxGaugeFace *gauge);
gboolean mtx_gauge_face_clear_peak(MtxGaugeFace *gauge);

G_END_DECLS

#endif
