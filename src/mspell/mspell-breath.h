﻿#pragma once

#include "system/angband.h"

struct MonsterSpellResult;

typedef struct player_type player_type;
MonsterSpellResult spell_RF4_BREATH(player_type *target_ptr, int GF_TYPE, POSITION y, POSITION x, MONSTER_IDX m_idx, MONSTER_IDX t_idx, int TARGET_TYPE);
