﻿#pragma once

#include "system/angband.h"
#include "lore/lore-util.h"

typedef struct player_type player_type;
void process_monster_lore(player_type *player_ptr, MONRACE_IDX r_idx, monster_lore_mode mode);
