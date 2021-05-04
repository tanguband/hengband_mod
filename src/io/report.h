﻿#pragma once

#include "system/angband.h"

extern concptr screen_dump;

#ifdef WORLD_SCORE
#include "io/files-util.h"

typedef struct player_type player_type;
errr report_score(player_type *creature_ptr, display_player_pf display_player);
concptr make_screen_dump(player_type *creature_ptr);
#endif
