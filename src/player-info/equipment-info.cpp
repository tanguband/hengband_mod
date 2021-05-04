﻿#include "player-info/equipment-info.h"
#include "inventory/inventory-slot-types.h"
#include "object-hook/hook-weapon.h"
#include "pet/pet-util.h"
#include "player-status/player-hand-types.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"

/*!
 * @brief プレイヤーが現在右手/左手に武器を持っているか判定する /
 * @param i 判定する手のID(右手:INVEN_MAIN_HAND 左手:INVEN_SUB_HAND)
 * @return 持っているならばTRUE
 */
bool has_melee_weapon(player_type *creature_ptr, int slot)
{
    return ((creature_ptr->inventory_list[slot].k_idx) && object_is_melee_weapon(&creature_ptr->inventory_list[slot]));
}

/*!
 * @brief プレイヤーの現在開いている手の状態を返す
 * @param riding_control 乗馬中により片手を必要としている状態ならばTRUEを返す。
 * @return 開いている手のビットフラグ
 */
BIT_FLAGS16 empty_hands(player_type *creature_ptr, bool riding_control)
{
    BIT_FLAGS16 status = EMPTY_HAND_NONE;
    if (!creature_ptr->inventory_list[INVEN_MAIN_HAND].k_idx)
        status |= EMPTY_HAND_MAIN;
    if (!creature_ptr->inventory_list[INVEN_SUB_HAND].k_idx)
        status |= EMPTY_HAND_SUB;

    if (riding_control && (status != EMPTY_HAND_NONE) && creature_ptr->riding && none_bits(creature_ptr->pet_extra_flags, PF_TWO_HANDS)) {
        if (any_bits(status, EMPTY_HAND_SUB))
            reset_bits(status, EMPTY_HAND_SUB);
        else if (any_bits(status, EMPTY_HAND_MAIN))
            reset_bits(status, EMPTY_HAND_MAIN);
    }

    return status;
}

bool can_two_hands_wielding(player_type *creature_ptr)
{
    return !creature_ptr->riding || any_bits(creature_ptr->pet_extra_flags, PF_TWO_HANDS);
}

/*!
 * @brief プレイヤーが防具重量制限のある職業時にペナルティを受ける状態にあるかどうかを返す。
 * @return ペナルティが適用されるならばTRUE。
 */
bool heavy_armor(player_type *creature_ptr)
{
    if ((creature_ptr->pclass != CLASS_MONK) && (creature_ptr->pclass != CLASS_FORCETRAINER) && (creature_ptr->pclass != CLASS_NINJA))
        return FALSE;

    WEIGHT monk_arm_wgt = 0;
    if (creature_ptr->inventory_list[INVEN_MAIN_HAND].tval > TV_SWORD)
        monk_arm_wgt += creature_ptr->inventory_list[INVEN_MAIN_HAND].weight;
    if (creature_ptr->inventory_list[INVEN_SUB_HAND].tval > TV_SWORD)
        monk_arm_wgt += creature_ptr->inventory_list[INVEN_SUB_HAND].weight;
    monk_arm_wgt += creature_ptr->inventory_list[INVEN_BODY].weight;
    monk_arm_wgt += creature_ptr->inventory_list[INVEN_HEAD].weight;
    monk_arm_wgt += creature_ptr->inventory_list[INVEN_OUTER].weight;
    monk_arm_wgt += creature_ptr->inventory_list[INVEN_ARMS].weight;
    monk_arm_wgt += creature_ptr->inventory_list[INVEN_FEET].weight;

    return (monk_arm_wgt > (100 + (creature_ptr->lev * 4)));
}
