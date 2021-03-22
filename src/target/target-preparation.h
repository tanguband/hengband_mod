#pragma once

#include <vector>

#include "system/angband.h"

bool target_able(player_type *creature_ptr, MONSTER_IDX m_idx);
void target_set_prepare(player_type *creature_ptr, std::vector<POSITION> &ys, std::vector<POSITION> &xs, BIT_FLAGS mode);
void target_sensing_monsters_prepare(player_type *creature_ptr, std::vector<MONSTER_IDX> &monster_list);
