﻿#include "object-hook/hook-magic.h"
#include "inventory/inventory-slot-types.h"
#include "object-enchant/tr-types.h"
#include "object/object-flags.h"
#include "perception/object-perception.h"
#include "player-info/class-info.h"
#include "player/player-realm.h"
#include "realm/realm-names-table.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "util/enum-converter.h"

/*!
 * @brief オブジェクトをプレイヤーが簡易使用コマンドで利用できるかを判定する /
 * Hook to determine if an object is useable
 * @param o_ptr 判定したいオブジェクトの構造体参照ポインタ
 * @return 利用可能ならばTRUEを返す
 */
bool item_tester_hook_use(player_type *player_ptr, const object_type *o_ptr)
{
    if (o_ptr->tval == player_ptr->tval_ammo)
        return true;

    switch (o_ptr->tval) {
    case TV_SPIKE:
    case TV_STAFF:
    case TV_WAND:
    case TV_ROD:
    case TV_SCROLL:
    case TV_POTION:
    case TV_FOOD: {
        return true;
    }

    default: {
        int i;

        if (!o_ptr->is_known())
            return false;
        for (i = INVEN_MAIN_HAND; i < INVEN_TOTAL; i++) {
            if (&player_ptr->inventory_list[i] == o_ptr) {
                auto flags = object_flags(o_ptr);
                if (flags.has(TR_ACTIVATE))
                    return true;
            }
        }
    }
    }

    return false;
}

/*!
 * @brief オブジェクトがプレイヤーが使用可能な魔道書かどうかを判定する
 * @param o_ptr 判定したいオブ会ジェクトの構造体参照ポインタ
 * @return 学習できる魔道書ならばTRUEを返す
 */
bool item_tester_learn_spell(player_type *player_ptr, const object_type *o_ptr)
{
    int32_t choices = realm_choices2[player_ptr->pclass];
    if (player_ptr->pclass == CLASS_PRIEST) {
        if (is_good_realm(player_ptr->realm1)) {
            choices &= ~(CH_DEATH | CH_DAEMON);
        } else {
            choices &= ~(CH_LIFE | CH_CRUSADE);
        }
    }

    if ((o_ptr->tval < TV_LIFE_BOOK) || (o_ptr->tval > (TV_LIFE_BOOK + enum2i(MAX_REALM) - 1)))
        return false;

    if ((o_ptr->tval == TV_MUSIC_BOOK) && (player_ptr->pclass == CLASS_BARD))
        return true;
    else if (!is_magic(tval2realm(o_ptr->tval)))
        return false;

    return (get_realm1_book(player_ptr) == o_ptr->tval) || (get_realm2_book(player_ptr) == o_ptr->tval)
        || (choices & (0x0001U << (tval2realm(o_ptr->tval) - 1)));
}

/*!
 * @brief オブジェクトが高位の魔法書かどうかを判定する
 * @param o_ptr 判定したいオブジェクトの構造体参照ポインタ
 * @return オブジェクトが高位の魔法書ならばTRUEを返す
 */
bool item_tester_high_level_book(const object_type *o_ptr)
{
    if ((o_ptr->tval == TV_LIFE_BOOK) || (o_ptr->tval == TV_SORCERY_BOOK) || (o_ptr->tval == TV_NATURE_BOOK) || (o_ptr->tval == TV_CHAOS_BOOK)
        || (o_ptr->tval == TV_DEATH_BOOK) || (o_ptr->tval == TV_TRUMP_BOOK) || (o_ptr->tval == TV_CRAFT_BOOK) || (o_ptr->tval == TV_DEMON_BOOK)
        || (o_ptr->tval == TV_CRUSADE_BOOK) || (o_ptr->tval == TV_MUSIC_BOOK) || (o_ptr->tval == TV_HEX_BOOK)) {
        if (o_ptr->sval > 1)
            return true;
        else
            return false;
    }

    return false;
}
