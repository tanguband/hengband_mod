﻿#pragma once
/*!
 * @file learnt-power-getter.h
 * @brief 青魔法の処理実行ヘッダ
 */

#include "system/angband.h"

typedef struct player_type player_type;
bool get_learned_power(player_type *caster_ptr, SPELL_IDX *sn);
