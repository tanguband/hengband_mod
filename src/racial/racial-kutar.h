﻿#pragma once

#include "system/angband.h"

typedef struct player_type player_type;
bool set_leveling(player_type *creature_ptr, TIME_EFFECT v, bool do_dec);
