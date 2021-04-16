﻿/*!
 * @brief プレイヤーの食べるコマンド実装
 * @date 2018/09/07
 @ @author deskull
 */

#include "cmd-item/cmd-eat.h"
#include "core/hp-mp-processor.h"
#include "core/player-update-types.h"
#include "core/window-redrawer.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "floor/floor-object.h"
#include "inventory/inventory-object.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-race/monster-race.h"
#include "object-enchant/special-object-flags.h"
#include "object-hook/hook-expendable.h"
#include "object/item-tester-hooker.h"
#include "object/item-use-flags.h"
#include "object/object-generator.h"
#include "object/object-info.h"
#include "object/object-kind-hook.h"
#include "object/object-kind.h"
#include "perception/object-perception.h"
#include "player/attack-defense-types.h"
#include "player-info/avatar.h"
#include "player/digestion-processor.h"
#include "player/mimic-info-table.h"
#include "player/player-class.h"
#include "player/player-damage.h"
#include "player/player-race-types.h"
#include "player/special-defense-types.h"
#include "player/player-status-flags.h"
#include "spell-realm/spells-hex.h"
#include "spell/spells-status.h"
#include "status/action-setter.h"
#include "status/bad-status-setter.h"
#include "status/base-status.h"
#include "status/element-resistance.h"
#include "status/experience.h"
#include "sv-definition/sv-food-types.h"
#include "sv-definition/sv-other-types.h"
#include "util/string-processor.h"
#include "view/display-messages.h"
#include "view/object-describer.h"

/*!
 * @brief 食料タイプの食料を食べたときの効果を発動
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @return 鑑定されるならTRUE、されないならFALSE
 */
bool exe_eat_food_type_object(player_type *creature_ptr, object_type *o_ptr)
{
    if (o_ptr->tval != TV_FOOD)
        return FALSE;

    switch (o_ptr->sval) {
    case SV_FOOD_POISON:
        if (!(has_resist_pois(creature_ptr) || is_oppose_pois(creature_ptr)))
            if (set_poisoned(creature_ptr, creature_ptr->poisoned + randint0(10) + 10))
                return TRUE;
        break;
    case SV_FOOD_BLINDNESS:
        if (!has_resist_blind(creature_ptr))
            if (set_blind(creature_ptr, creature_ptr->blind + randint0(200) + 200))
                return TRUE;
        break;
    case SV_FOOD_PARANOIA:
        if (!has_resist_fear(creature_ptr))
            if (set_afraid(creature_ptr, creature_ptr->afraid + randint0(10) + 10))
                return TRUE;
        break;
    case SV_FOOD_CONFUSION:
        if (!has_resist_conf(creature_ptr))
            if (set_confused(creature_ptr, creature_ptr->confused + randint0(10) + 10))
                return TRUE;
        break;
    case SV_FOOD_HALLUCINATION:
        if (!has_resist_chaos(creature_ptr))
            if (set_image(creature_ptr, creature_ptr->image + randint0(250) + 250))
                return TRUE;
        break;
    case SV_FOOD_PARALYSIS:
        if (!creature_ptr->free_act)
            if (set_paralyzed(creature_ptr, creature_ptr->paralyzed + randint0(10) + 10))
                return TRUE;
        break;
    case SV_FOOD_WEAKNESS:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(6, 6), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_STR);
        return TRUE;
    case SV_FOOD_SICKNESS:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(6, 6), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_CON);
        return TRUE;
    case SV_FOOD_STUPIDITY:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(8, 8), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_INT);
        return TRUE;
    case SV_FOOD_NAIVETY:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(8, 8), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_WIS);
        return TRUE;
    case SV_FOOD_UNHEALTH:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(10, 10), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_CON);
        return TRUE;
    case SV_FOOD_DISEASE:
        take_hit(creature_ptr, DAMAGE_NOESCAPE, damroll(10, 10), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(creature_ptr, A_STR);
        return TRUE;
    case SV_FOOD_CURE_POISON:
        if (set_poisoned(creature_ptr, 0))
            return TRUE;
        break;
    case SV_FOOD_CURE_BLINDNESS:
        if (set_blind(creature_ptr, 0))
            return TRUE;
        break;
    case SV_FOOD_CURE_PARANOIA:
        if (set_afraid(creature_ptr, 0))
            return TRUE;
        break;
    case SV_FOOD_CURE_CONFUSION:
        if (set_confused(creature_ptr, 0))
            return TRUE;
        break;
    case SV_FOOD_CURE_SERIOUS:
        return cure_serious_wounds(creature_ptr, 4, 8);
    case SV_FOOD_RESTORE_STR:
        if (do_res_stat(creature_ptr, A_STR))
            return TRUE;
        break;
    case SV_FOOD_RESTORE_CON:
        if (do_res_stat(creature_ptr, A_CON))
            return TRUE;
        break;
    case SV_FOOD_RESTORING:
        return restore_all_status(creature_ptr);
#ifdef JP
    /* それぞれの食べ物の感想をオリジナルより細かく表現 */
    case SV_FOOD_BISCUIT:
        msg_print("甘くてサクサクしてとてもおいしい。");
        return TRUE;
    case SV_FOOD_JERKY:
        msg_print("歯ごたえがあっておいしい。");
        return TRUE;
    case SV_FOOD_SLIME_MOLD:
        msg_print("これはなんとも形容しがたい味だ。");
        return TRUE;
    case SV_FOOD_RATION:
        msg_print("これはおいしい。");
        return TRUE;
#else
    case SV_FOOD_RATION:
    case SV_FOOD_BISCUIT:
    case SV_FOOD_JERKY:
    case SV_FOOD_SLIME_MOLD:
        msg_print("That tastes good.");
        return TRUE;
#endif
    case SV_FOOD_WAYBREAD:
        msg_print(_("これはひじょうに美味だ。", "That tastes good."));
        (void)set_poisoned(creature_ptr, 0);
        (void)hp_player(creature_ptr, damroll(4, 8));
        return TRUE;
    case SV_FOOD_PINT_OF_ALE:
    case SV_FOOD_PINT_OF_WINE:
        msg_print(_("のどごし爽やかだ。", "That tastes good."));
        return TRUE;
    }

    return FALSE;
}

/*!
 * @brief 魔法道具のチャージをの食料として食べたときの効果を発動
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @param item オブジェクトのインベントリ番号
 * @return 食べようとしたらTRUE、しなかったらFALSE
 */
bool exe_eat_charge_of_magic_device(player_type *creature_ptr, object_type *o_ptr, INVENTORY_IDX item)
{
    if (o_ptr->tval != TV_STAFF && o_ptr->tval != TV_WAND)
        return FALSE;

    if (is_specific_player_race(creature_ptr, RACE_SKELETON) || is_specific_player_race(creature_ptr, RACE_GOLEM)
            || is_specific_player_race(creature_ptr, RACE_ZOMBIE) || is_specific_player_race(creature_ptr, RACE_SPECTRE)) {
        concptr staff;

        if (o_ptr->tval == TV_STAFF && (item < 0) && (o_ptr->number > 1)) {
            msg_print(_("まずは杖を拾わなければ。", "You must first pick up the staffs."));
            return TRUE;
        }

        staff = (o_ptr->tval == TV_STAFF) ? _("杖", "staff") : _("魔法棒", "wand");

        /* "Eat" charges */
        if (o_ptr->pval == 0) {
            msg_format(_("この%sにはもう魔力が残っていない。", "The %s has no charges left."), staff);
            o_ptr->ident |= (IDENT_EMPTY);
            creature_ptr->window_flags |= (PW_INVEN);
            return TRUE;
        }

        msg_format(_("あなたは%sの魔力をエネルギー源として吸収した。", "You absorb mana of the %s as your energy."), staff);

        /* Use a single charge */
        o_ptr->pval--;

        /* Eat a charge */
        set_food(creature_ptr, creature_ptr->food + 5000);

        /* XXX Hack -- unstack if necessary */
        if (o_ptr->tval == TV_STAFF && (item >= 0) && (o_ptr->number > 1)) {
            object_type forge;
            object_type *q_ptr;
            q_ptr = &forge;
            object_copy(q_ptr, o_ptr);

            /* Modify quantity */
            q_ptr->number = 1;

            /* Restore the charges */
            o_ptr->pval++;

            /* Unstack the used item */
            o_ptr->number--;
            item = store_item_to_inventory(creature_ptr, q_ptr);

            msg_format(_("杖をまとめなおした。", "You unstack your staff."));
        }

        if (item >= 0) {
            inven_item_charges(creature_ptr, item);
        } else {
            floor_item_charges(creature_ptr->current_floor_ptr, 0 - item);
        }

        creature_ptr->window_flags |= (PW_INVEN | PW_EQUIP);
        return TRUE;
    }

    return FALSE;
}

/*!
 * @brief 食料を食べるコマンドのサブルーチン
 * @param item 食べるオブジェクトの所持品ID
 * @return なし
 */
void exe_eat_food(player_type *creature_ptr, INVENTORY_IDX item)
{
    if (music_singing_any(creature_ptr))
        stop_singing(creature_ptr);
    if (hex_spelling_any(creature_ptr))
        stop_hex_spell_all(creature_ptr);

    object_type *o_ptr = ref_item(creature_ptr, item);

    sound(SOUND_EAT);

    take_turn(creature_ptr, 100);

    /* Object level */
    int lev = k_info[o_ptr->k_idx].level;

    /* Identity not known yet */
    int ident = exe_eat_food_type_object(creature_ptr, o_ptr);

    /*
     * Store what may have to be updated for the inventory (including
     * autodestroy if set by something else).  Then turn off those flags
     * so that updates triggered by calling gain_exp() or set_food() below
     * do not rearrange the inventory before the food item is destroyed in
     * the pack.
     */
    BIT_FLAGS inventory_flags = (PU_COMBINE | PU_REORDER | (creature_ptr->update & PU_AUTODESTROY));
    creature_ptr->update &= ~(PU_COMBINE | PU_REORDER | PU_AUTODESTROY);

    if (!(object_is_aware(o_ptr))) {
        chg_virtue(creature_ptr, V_KNOWLEDGE, -1);
        chg_virtue(creature_ptr, V_PATIENCE, -1);
        chg_virtue(creature_ptr, V_CHANCE, 1);
    }

    /* We have tried it */
    if (o_ptr->tval == TV_FOOD)
        object_tried(o_ptr);

    /* The player is now aware of the object */
    if (ident && !object_is_aware(o_ptr)) {
        object_aware(creature_ptr, o_ptr);
        gain_exp(creature_ptr, (lev + (creature_ptr->lev >> 1)) / creature_ptr->lev);
    }

    creature_ptr->window_flags |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

    /* Undeads drain recharge of magic device */
    if (exe_eat_charge_of_magic_device(creature_ptr, o_ptr, item)) {
        creature_ptr->update |= inventory_flags;
        return;
    }

    /* Balrogs change humanoid corpses to energy */
    if ((is_specific_player_race(creature_ptr, RACE_BALROG) || (mimic_info[creature_ptr->mimic_form].MIMIC_FLAGS & MIMIC_IS_DEMON))
        && (o_ptr->tval == TV_CORPSE && o_ptr->sval == SV_CORPSE && angband_strchr("pht", r_info[o_ptr->pval].d_char))) {
        GAME_TEXT o_name[MAX_NLEN];
        describe_flavor(creature_ptr, o_name, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));
        msg_format(_("%sは燃え上り灰になった。精力を吸収した気がする。", "%^s is burnt to ashes.  You absorb its vitality!"), o_name);
        (void)set_food(creature_ptr, PY_FOOD_MAX - 1);

        creature_ptr->update |= inventory_flags;
        vary_item(creature_ptr, item, -1);
        return;
    }

    if (is_specific_player_race(creature_ptr, RACE_SKELETON)) {
        if (!((o_ptr->sval == SV_FOOD_WAYBREAD) || (o_ptr->sval < SV_FOOD_BISCUIT))) {
            object_type forge;
            object_type *q_ptr = &forge;

            msg_print(_("食べ物がアゴを素通りして落ちた！", "The food falls through your jaws!"));
            object_prep(creature_ptr, q_ptr, lookup_kind(o_ptr->tval, o_ptr->sval));

            /* Drop the object from heaven */
            (void)drop_near(creature_ptr, q_ptr, -1, creature_ptr->y, creature_ptr->x);
        } else {
            msg_print(_("食べ物がアゴを素通りして落ち、消えた！", "The food falls through your jaws and vanishes!"));
        }
    } else if (is_specific_player_race(creature_ptr, RACE_VAMPIRE) || (creature_ptr->mimic_form == MIMIC_VAMPIRE)) {
        /* Vampires are filled only by bloods, so reduced nutritional benefit */
        (void)set_food(creature_ptr, creature_ptr->food + (o_ptr->pval / 10));
        msg_print(_("あなたのような者にとって食糧など僅かな栄養にしかならない。", "Mere victuals hold scant sustenance for a being such as yourself."));

        if (creature_ptr->food < PY_FOOD_ALERT) /* Hungry */
            msg_print(_("あなたの飢えは新鮮な血によってのみ満たされる！", "Your hunger can only be satisfied with fresh blood!"));
    } else if (is_specific_player_race(creature_ptr, RACE_GOLEM) || is_specific_player_race(creature_ptr, RACE_ZOMBIE)
        || is_specific_player_race(creature_ptr, RACE_ENT) || is_specific_player_race(creature_ptr, RACE_BALROG)
        || is_specific_player_race(creature_ptr, RACE_ANDROID) || is_specific_player_race(creature_ptr, RACE_SPECTRE)
        || (mimic_info[creature_ptr->mimic_form].MIMIC_FLAGS & MIMIC_IS_NONLIVING)) {
        msg_print(_("生者の食物はあなたにとってほとんど栄養にならない。", "The food of mortals is poor sustenance for you."));
        set_food(creature_ptr, creature_ptr->food + ((o_ptr->pval) / 20));
    } else {
        if (o_ptr->tval == TV_FOOD && o_ptr->sval == SV_FOOD_WAYBREAD) {
            /* Waybread is always fully satisfying. */
            set_food(creature_ptr, MAX(creature_ptr->food, PY_FOOD_MAX - 1));
        } else {
            /* Food can feed the player */
            (void)set_food(creature_ptr, creature_ptr->food + o_ptr->pval);
        }
    }

    creature_ptr->update |= inventory_flags;
    vary_item(creature_ptr, item, -1);
}

/*!
 * @brief 食料を食べるコマンドのメインルーチン /
 * Eat some food (from the pack or floor)
 * @return なし
 */
void do_cmd_eat_food(player_type *creature_ptr)
{
    OBJECT_IDX item;
    concptr q, s;

    if (creature_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN)) {
        set_action(creature_ptr, ACTION_NONE);
    }

    item_tester_hook = item_tester_hook_eatable;

    q = _("どれを食べますか? ", "Eat which item? ");
    s = _("食べ物がない。", "You have nothing to eat.");

    if (!choose_object(creature_ptr, &item, q, s, (USE_INVEN | USE_FLOOR), TV_NONE))
        return;

    exe_eat_food(creature_ptr, item);
}
