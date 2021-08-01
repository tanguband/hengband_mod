﻿#pragma once

#include "object-enchant/enchanter-base.h"
#include "object-enchant/apply-magic-armors-base.h"
#include "system/angband.h"

struct object_type;
struct player_type;
class ArmorEnchanter : ArmorEnchanterBase {
public:
    ArmorEnchanter(player_type *owner_ptr, object_type *o_ptr, DEPTH level, int power);
    ArmorEnchanter() = delete;
    virtual ~ArmorEnchanter() = default;
    void apply_magic() override;

protected:
    void enchant(){};
    void give_ego_index() override;
    void give_high_ego_index(){};
    void give_cursed() override;

private:
    player_type *owner_ptr;
};
