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

#ifndef __UNIONS_H__
#define __UNIONS_H__


/* Megasquirt unions defined in C provided by Perry Harrington */

/* Differing endian systems need to proper setups for bitfields.  
 * Intel is little endian (LSB) Sparc is Big Endian, (MSB) as well
 * as a good number of PDA's that run linux... 
 */

/* unions used in the various structures... */
#ifdef MSB_BITFIELD /* PDA's possibly? */
/* Big endian systems (MSB) */
union squirt
{
        unsigned char   value;
        struct
        {
                unsigned char reserved  :2;
                unsigned char firing2   :1;     /* 0 = not squirting 1 = squirti
                                                   ng */
                unsigned char sched2    :1;     /* 0 = nothing scheduled 1 = sch
                                                   eduled to squirt */
                unsigned char firing1   :1;     /* 0 = not squirting 1 = squirti
                                                   ng */
                unsigned char sched1    :1;     /* 0 = nothing scheduled 1 = sch
                                                   eduled to squirt */
                unsigned char inj2      :1;     /* 0 = no squirt 1 = squirt */
                unsigned char inj1      :1;     /* 0 = no squirt 1 = squirt */
        } bit;
};
/* Big endian systems (MSB) */
union engine
{
        unsigned char      value;
        struct
        {
                unsigned char reserved  :1;
                unsigned char mapaen    :1;     /* 0 = not in MAP acceleration m
                                                   ode 1 = MAP deaceeleration mode */
                unsigned char tpsden    :1;     /* 0 = not in deacceleration mod
                                                   e 1 = in deacceleration mode */
                unsigned char tpsaen    :1;     /* 0 = not in TPS acceleration m
                                                   ode 1 = TPS acceleration mode */
                unsigned char warmup    :1;     /* 0 = not in warmup 1 = in warm
                                                   up */
                unsigned char startw    :1;     /* 0 = not in startup warmup 1 =
                                                   in warmup enrichment */
                unsigned char crank     :1;     /* 0 = engine not cranking 1 = e
                                                   ngine cranking */
                unsigned char running   :1;     /* 0 = engine not running 1 = ru
                                                   nning */
        } bit;
};

/* Big endian systems (MSB) */
union config11
{
        unsigned char      value;
        struct
        {
                unsigned char cylinders :4;     /* 0000 = 1 cyl 
                                                 * 0001 = 2 cyl
                                                 * 0010 = 3 cyl
                                                 * 0011 = 4 cyl
                                                 * 0100 = 5 cyl
                                                 * 0101 = 6 cyl
                                                 * 0110 = 7 cyl
                                                 * 0111 = 8 cyl
                                                 * 1000 = 9 cyl
                                                 * 1001 = 10 cyl
                                                 * 1010 = 11 cyl
                                                 * 1011 = 12 cyl
                                                 */
                unsigned char inj_type  :1;     /* 0 = multi-port, 1 = TBI */
                unsigned char eng_type  :1;     /* 0 = 4-stroke, 1 = 2-stroke */
                unsigned char map_type  :2;     /* 00 115KPA, 01-250kpa, 10,11
                                                 * user-defined */
        } bit;
};

/* Big endian systems (MSB) */
union config12
{
        unsigned char      value;
        struct
        {
                unsigned char injectors :4;     /* 0000 = 1 injector 
                                                 * 0001 = 2 injectors
                                                 * 0010 = 3 injectors
                                                 * 0011 = 4 injectors
                                                 * 0100 = 5 injectors
                                                 * 0101 = 6 injectors
                                                 * 0110 = 7 injectors
                                                 * 0111 = 8 injectors
                                                 * 1000 = 9 injectors
                                                 * 1001 = 10 injectors
                                                 * 1010 = 11 injectors
                                                 * 1011 = 12 injectors
                                                 */
                unsigned char mat_type  :2;     /* 00 = GM,
                                                 * 01,10,11 = user defined */
                unsigned char clt_type  :2;     /* 00 = GM, 
                                                 * 01,10,11 = user defined */
        } bit;
};

/* Big endian systems (MSB) */
union config13
{
        unsigned char      value;
        struct
        {
                unsigned char unused:3;         /* Last 3 bits... */
                /*DUALTABLE CODE SPECIFIC*/
                unsigned char idle_policy:1;    /* 0 = B%G style, */
                unsigned char baro_corr :1;     /* 0 = Enrichment off (100%) */
                                                /* 1 = Enrichment on  */
                                                /* 1 = Brian fielding PWM */
                unsigned char inj_strat :1;     /* 0 = SD, 1 = Alpha-N */
                unsigned char ego_type  :1;     /* 0 = narrow, 1=wide */
                unsigned char firing    :1;     /* 0 = normal, 1=odd fire */

        } bit;
};

#else
/* Little Endian systems (LSB), intel x86) */
union squirt
{
        unsigned char      value;
        struct
        {
                unsigned char inj1      :1;     /* 0 = no squirt 1 = squirt */
                unsigned char inj2      :1;     /* 0 = no squirt 1 = squirt */
                unsigned char sched1    :1;     /* 0 = nothing scheduled 1 = sch
                                                   eduled to squirt */
                unsigned char firing1   :1;     /* 0 = not squirting 1 = squirti
                                                   ng */
                unsigned char sched2    :1;     /* 0 = nothing scheduled 1 = sch
                                                   eduled to squirt */
                unsigned char firing2   :1;     /* 0 = not squirting 1 = squirti
                                                   ng */
                unsigned char reserved  :2;
        } bit;
};
/* Little Endian systems (LSB), intel x86) */
union engine
{
        unsigned char      value;
        struct
        {
                unsigned char running   :1;     /* 0 = engine not running 1 = running */
                unsigned char crank     :1;     /* 0 = engine not cranking 1 = engine cranking */
                unsigned char startw    :1;     /* 0 = not in startup warmup 1 = in warmup enrichment */
                unsigned char warmup    :1;     /* 0 = not in warmup 1 = in warmup */
                unsigned char tpsaen    :1;     /* 0 = not in TPS acceleration mode 1 = TPS acceleration mode */
                unsigned char tpsden    :1;     /* 0 = not in deacceleration mode 1 = in deacceleration mode */
                unsigned char mapaen    :1;     /* 0 = not in MAP acceleration mode 1 = MAP deaceeleration mode */

                unsigned char reserved  :1;
        } bit;
};
/* Little Endian systems (LSB), intel x86) */
union config11
{
        unsigned char      value;
        struct
        {
                unsigned char map_type  :2;     /* 00 115KPA, 01-250kpa, 10,11
                                                 * user-defined */
                unsigned char eng_type  :1;     /* 0 = 4-stroke, 1 = 2-stroke */
                unsigned char inj_type  :1;     /* 0 = multi-port, 1 = TBI */
                unsigned char cylinders :4;     /* 0000 = 1 cyl 
                                                 * 0001 = 2 cyl
                                                 * 0010 = 3 cyl
                                                 * 0011 = 4 cyl
                                                 * 0100 = 5 cyl
                                                 * 0101 = 6 cyl
                                                 * 0110 = 7 cyl
                                                 * 0111 = 8 cyl
                                                 * 1000 = 9 cyl
                                                 * 1001 = 10 cyl
                                                 * 1010 = 11 cyl
                                                 * 1011 = 12 cyl
                                                 */
        } bit;
};
/* Little Endian systems (LSB), intel x86) */
union config12
{
        unsigned char      value;
        struct
        {
                unsigned char clt_type  :2;     /* 00 = GM, 
                                                 * 01,10,11 = user defined */
                unsigned char mat_type  :2;     /* 00 = GM,
                                                 * 01,10,11 = user defined */
                unsigned char injectors :4;     /* 0000 = 1 injector 
                                                 * 0001 = 2 injectors
                                                 * 0010 = 3 injectors
                                                 * 0011 = 4 injectors
                                                 * 0100 = 5 injectors
                                                 * 0101 = 6 injectors
                                                 * 0110 = 7 injectors
                                                 * 0111 = 8 injectors
                                                 * 1000 = 9 injectors
                                                 * 1001 = 10 injectors
                                                 * 1010 = 11 injectors
                                                 * 1011 = 12 injectors
                                                 */
        } bit;
};
/* Little Endian systems (LSB), intel x86) */
union config13
{
        unsigned char      value;
        struct
        {
                unsigned char firing    :1;     /* 0 = normal, 1=odd fire */
                unsigned char ego_type  :1;     /* 0 = narrow, 1=wide */
                unsigned char inj_strat :1;     /* 0 = SD, 1 = Alpha-N */
                unsigned char baro_corr :1;     /* 0 = Enrichment off (100%) */
                                                /* 1 = Enrichment on  */
                /*DUALTABLE CODE SPECIFIC*/
                unsigned char idle_policy:1;    /* 0 = B%G style, */
                                                /* 1 = Brian fielding PWM */

                unsigned char unused:3;         /* Last 3 bits... */
        } bit;
};
#endif

#endif