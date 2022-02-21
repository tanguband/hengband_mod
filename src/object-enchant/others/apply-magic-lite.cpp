﻿#include "object-enchant/others/apply-magic-lite.h"
#include "artifact/random-art-generator.h"
#include "inventory/inventory-slot-types.h"
#include "object-enchant/object-ego.h"
#include "sv-definition/sv-lite-types.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"

LiteEnchanter::LiteEnchanter(PlayerType *player_ptr, ObjectType *o_ptr, int power)
    : player_ptr(player_ptr)
    , o_ptr(o_ptr)
    , power(power)
{
    switch (o_ptr->sval) {
    case SV_LITE_TORCH:
        if (o_ptr->pval > 0) {
            o_ptr->fuel = randint1(o_ptr->pval);
        }

        o_ptr->pval = 0;
        return;
    case SV_LITE_LANTERN:
        if (o_ptr->pval > 0) {
            o_ptr->fuel = randint1(o_ptr->pval);
        }

        o_ptr->pval = 0;
        return;
    default:
        return;
    }
}

void LiteEnchanter::apply_magic()
{
    if (this->power > 2) {
        become_random_artifact(this->player_ptr, this->o_ptr, false);
        return;
    }
    
    if ((this->power == 2) || ((this->power == 1) && one_in_(3))) {
        while (this->o_ptr->name2 == 0) {
            this->give_ego_index();
        }

        return;
    }
    
    if (this->power == -2) {
        this->give_cursed();
    }
}

void LiteEnchanter::give_ego_index()
{
    while (true) {
        auto okay_flag = true;
        this->o_ptr->name2 = get_random_ego(INVEN_LITE, true);
        switch (this->o_ptr->name2) {
        case EGO_LITE_LONG:
            if (this->o_ptr->sval == SV_LITE_FEANOR) {
                okay_flag = false;
            }

            break;
        default:
            break;
        }

        if (okay_flag) {
            return;
        }
    }
}

void LiteEnchanter::give_cursed()
{
    this->o_ptr->name2 = get_random_ego(INVEN_LITE, false);
    switch (this->o_ptr->name2) {
    case EGO_LITE_DARKNESS:
        this->o_ptr->fuel = 0;
        this->add_dark_flag();
        return;
    default:
        return;
    }
}

void LiteEnchanter::add_dark_flag()
{
    switch (this->o_ptr->sval) {
    case SV_LITE_TORCH:
        this->o_ptr->art_flags.set(TR_LITE_M1);
        return;
    case SV_LITE_LANTERN:
        this->o_ptr->art_flags.set(TR_LITE_M2);
        return;
    case SV_LITE_FEANOR:
        this->o_ptr->art_flags.set(TR_LITE_M3);
        return;
    default:
        return;
    }
}
