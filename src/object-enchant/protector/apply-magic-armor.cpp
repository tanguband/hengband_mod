﻿/*
 * @brief 鎧類に耐性等の追加効果を付与する処理
 * @date 2021/08/01
 * @author Hourier
 */

#include "object-enchant/protector/apply-magic-armor.h"
#include "artifact/random-art-generator.h"
#include "inventory/inventory-slot-types.h"
#include "object-enchant/object-ego.h"
#include "object/object-kind-hook.h"
#include "player/player-personality-types.h"
#include "sv-definition/sv-armor-types.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "view/display-messages.h"

/*
 * @brief コンストラクタ
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param o_ptr 強化を与えたいオブジェクトの構造体参照ポインタ
 * @param level 生成基準階
 * @param power 生成ランク
 */
ArmorEnchanter::ArmorEnchanter(PlayerType *player_ptr, ObjectType *o_ptr, DEPTH level, int power)
    : AbstractProtectorEnchanter{ o_ptr, level, power }
    , player_ptr(player_ptr)
{
}

/*!
 * @brief power > 2 はデバッグ専用.
 */
void ArmorEnchanter::apply_magic()
{
    if (this->power == 0) {
        return;
    }

    switch (this->o_ptr->tval) {
    case ItemKindType::DRAG_ARMOR:
        if ((this->power > 2) || one_in_(50)) {
            become_random_artifact(this->player_ptr, this->o_ptr, false);
        }

        break;
    case ItemKindType::HARD_ARMOR:
        if (this->power > 1) {
            this->give_ego_index();
            return;
        }

        if (this->power < -1) {
            this->give_cursed();
        }

        return;
    case ItemKindType::SOFT_ARMOR:
        // @todo 後ほどSoftArmorEnchanterへ分離した時、このswitch文はsval_enchant() へ移動させる.
        switch (this->o_ptr->sval) {
        case SV_KUROSHOUZOKU:
            this->o_ptr->pval = randint1(4);
            break;
        case SV_ABUNAI_MIZUGI:
            if (this->player_ptr->ppersonality != PERSONALITY_SEXY) {
                break;
            }

            this->o_ptr->pval = 3;
            this->o_ptr->art_flags.set(TR_STR);
            this->o_ptr->art_flags.set(TR_INT);
            this->o_ptr->art_flags.set(TR_WIS);
            this->o_ptr->art_flags.set(TR_DEX);
            this->o_ptr->art_flags.set(TR_CON);
            this->o_ptr->art_flags.set(TR_CHR);
            break;
        default:
            break;
        }

        if (this->power > 1) {
            this->give_high_ego_index();
            if (this->is_high_ego_generated) {
                return;
            }

            this->give_ego_index();
            return;
        }

        if (this->power < -1) {
            this->give_cursed();
        }

        return;
    default:
        return;
    }
}

/*
 * @details power > 2はデバッグ専用.
 */
void ArmorEnchanter::give_ego_index()
{
    if ((this->power > 2) || one_in_(20)) {
        become_random_artifact(this->player_ptr, this->o_ptr, false);
        return;
    }

    while (true) {
        auto valid = true;
        this->o_ptr->ego_idx = get_random_ego(INVEN_BODY, true);
        switch (this->o_ptr->ego_idx) {
        case EgoType::DWARVEN:
            if (this->o_ptr->tval != ItemKindType::HARD_ARMOR) {
                valid = false;
            }

            break;
        case EgoType::DRUID:
            if (this->o_ptr->tval != ItemKindType::SOFT_ARMOR) {
                valid = false;
            }

            break;
        default:
            break;
        }

        if (valid) {
            break;
        }
    }
}

/*
 * @brief ベースアイテムがローブの時、確率で永続か宵闇のローブを生成する.
 * @return 生成条件を満たしたらtrue、満たさなかったらfalse
 * @details 永続：12%、宵闇：3%
 */
void ArmorEnchanter::give_high_ego_index()
{
    if ((this->o_ptr->sval != SV_ROBE) || (randint0(100) >= 15)) {
        return;
    }

    this->is_high_ego_generated = true;
    auto ego_robe = one_in_(5);
    this->o_ptr->ego_idx = ego_robe ? EgoType::TWILIGHT : EgoType::PERMANENCE;
    if (!ego_robe) {
        return;
    }

    this->o_ptr->k_idx = lookup_kind(ItemKindType::SOFT_ARMOR, SV_TWILIGHT_ROBE);
    this->o_ptr->sval = SV_TWILIGHT_ROBE;
    this->o_ptr->ac = 0;
    this->o_ptr->to_a = 0;
    return;
}

void ArmorEnchanter::give_cursed()
{
    this->o_ptr->ego_idx = get_random_ego(INVEN_BODY, false);
    switch (this->o_ptr->ego_idx) {
    case EgoType::A_DEMON:
    case EgoType::A_MORGUL:
        return;
    default:
        msg_print(_("エラー：適した呪い鎧エゴがみつかりませんでした.", "Error:  suitable cursed armor ego not found."));
        return;
    }
}
