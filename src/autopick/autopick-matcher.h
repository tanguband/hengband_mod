﻿#pragma once

#include "system/angband.h"

struct autopick_type;
class ObjectType;
class PlayerType;
bool is_autopick_match(PlayerType *player_ptr, ObjectType *o_ptr, autopick_type *entry, concptr o_name);
