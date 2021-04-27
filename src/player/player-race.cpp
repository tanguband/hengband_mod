﻿#include "player/player-race.h"
#include "core/player-redraw-types.h"
#include "inventory/inventory-slot-types.h"
#include "player/player-race-types.h"
#include "system/object-type-definition.h"
#include "mimic-info-table.h"
#include "player/race-info-table.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"

const player_race *rp_ptr;

const player_race *get_player_race_info(player_type* creature_ptr, bool base_race = false)
{
    if (base_race)
        return &race_info[creature_ptr->prace];

    switch (creature_ptr->mimic_form) {
    case MIMIC_DEMON:
    case MIMIC_DEMON_LORD:
    case MIMIC_VAMPIRE:
        return &mimic_info[creature_ptr->mimic_form];
    default: // MIMIC_NONE or undefined
        return &race_info[creature_ptr->prace];
    }
}

/*!
 * @brief 救援召喚時のモンスターシンボルを返す
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @return シンボル文字
 */
SYMBOL_CODE get_summon_symbol_from_player(player_type *creature_ptr)
{
    SYMBOL_CODE symbol = 'N';
    auto mmc_ptr = get_player_race_info(creature_ptr);

    auto l = strlen(mmc_ptr->symbol);
    auto mul = 1;
    for (size_t i = 0; i < l; i++) {
        if (one_in_(mul))
            symbol = mmc_ptr->symbol[i];
        mul *= 13;
    }
    return symbol;
}

bool is_specific_player_race(player_type *creature_ptr, player_race_type prace) { return (!creature_ptr->mimic_form && (creature_ptr->prace == prace)); }

/*!
 * @brief 種族が指定の耐性/能力フラグを持つか判定する
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param flag 判定するフラグ
 * @param base_race ベース種族の情報を返すならtrue、ミミック擬態中の種族を返すならfalse
 * @return 持つならtrue、持たないならfalse
 */
bool player_race_has_flag(player_type *creature_ptr, tr_type flag, bool base_race)
{
    auto race_ptr = get_player_race_info(creature_ptr, base_race);

    for (auto& cond : race_ptr->extra_flags) {
        if (cond.type != flag)
            continue;
        if (creature_ptr->lev < cond.level)
            continue;
        if (cond.pclass != std::nullopt) {
            if (cond.not_class && creature_ptr->pclass == cond.pclass)
                continue;
            if (!cond.not_class && creature_ptr->pclass != cond.pclass)
                continue;
        }

        return true;
    }

    return false;
}

/*!
 * @brief 種族固有の耐性/能力フラグをセットする
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param flags フラグ配列へのポインタ
 * @param base_race ベース種族の情報を返すならtrue、ミミック擬態中の種族を返すならfalse
 * @return なし
 */
void add_player_race_flags(player_type *creature_ptr, BIT_FLAGS *flags, bool base_race)
{
    auto race_ptr = get_player_race_info(creature_ptr, base_race);
    if (race_ptr->infra > 0)
        add_flag(flags, TR_INFRA);

    for (auto &cond : race_ptr->extra_flags) {
        if (creature_ptr->lev < cond.level)
            continue;
        if (cond.pclass != std::nullopt) {
            if (cond.not_class && creature_ptr->pclass == cond.pclass)
                continue;
            if (!cond.not_class && creature_ptr->pclass != cond.pclass)
                continue;
        }

        add_flag(flags, cond.type);
    }
}

/*!
 * @brief 種族の生命形態を返す
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @return 生命形態
 */
PlayerRaceLife player_race_life(player_type *creature_ptr, bool base_race)
{
    auto race_ptr = get_player_race_info(creature_ptr, base_race);
    return race_ptr->life;
}
