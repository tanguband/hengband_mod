﻿#pragma once

#include "system/angband.h"
#include "io/files-util.h"

typedef struct player_type player_type;
void dump_aux_player_status(player_type *creature_ptr, FILE *fff, display_player_pf display_player);
