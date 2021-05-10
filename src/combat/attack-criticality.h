﻿#pragma once

#include "combat/combat-options-type.h"
#include "system/angband.h"

#include <tuple>

typedef struct player_attack_type player_attack_type;
typedef struct player_type player_type;
std::tuple<HIT_POINT, concptr> apply_critical_norm_damage(int k, HIT_POINT base_dam);
HIT_POINT critical_norm(player_type *attacker_ptr, WEIGHT weight, int plus, HIT_POINT dam, s16b meichuu, combat_options mode, bool impact = false);
int calc_monster_critical(DICE_NUMBER dice, DICE_SID sides, HIT_POINT dam);
void critical_attack(player_type *attacker_ptr, player_attack_type *pa_ptr);
