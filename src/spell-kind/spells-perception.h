﻿#pragma once

#include "system/angband.h"
#include "object/tval-types.h"

typedef struct object_type object_type;
typedef struct player_type player_type;
void identify_pack(player_type *target_ptr);
bool identify_item(player_type *owner_ptr, object_type *o_ptr);
bool ident_spell(player_type *caster_ptr, bool only_equip, tval_type item_tester_tval);
bool identify_fully(player_type *caster_ptr, bool only_equip, tval_type item_tester_tval);
