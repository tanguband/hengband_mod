﻿#pragma once

#include "system/angband.h"
#include "combat/combat-options-type.h"

typedef struct monap_type monap_type;
typedef struct monster_type monster_type;
typedef struct player_attack_type player_attack_type;
typedef struct player_type player_type;
MULTIPLY mult_hissatsu(player_type *attacker_ptr, MULTIPLY mult, BIT_FLAGS *flgs, monster_type *m_ptr, combat_options mode);
void concentration(player_type *creature_ptr);
bool choose_kata(player_type* creature_ptr);
int calc_attack_quality(player_type *attacker_ptr, player_attack_type *pa_ptr);
void mineuchi(player_type *attacker_ptr, player_attack_type *pa_ptr);
void musou_counterattack(player_type *target_ptr, monap_type *monap_ptr);
