﻿#pragma once

#include "system/angband.h"

extern bool build_type16(player_type *player_ptr);

/* Minimum & maximum town size */
#define MIN_TOWN_WID ((MAX_WID / 3) / 2)
#define MIN_TOWN_HGT ((MAX_HGT / 3) / 2)
#define MAX_TOWN_WID ((MAX_WID / 3) * 2 / 3)
#define MAX_TOWN_HGT ((MAX_HGT / 3) * 2 / 3)

/* Struct for build underground buildings */
typedef struct
{
	POSITION y0, x0; /* North-west corner (relative) */
	POSITION y1, x1; /* South-east corner (relative) */
}
ugbldg_type;

ugbldg_type *ugbldg;
