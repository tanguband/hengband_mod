﻿#pragma once

#include "system/angband.h"
#include "io/files-util.h"

typedef struct player_type player_type;
void make_character_dump(player_type *creature_ptr, FILE *fff, display_player_pf display_player);
