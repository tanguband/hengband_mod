﻿/*!
 * @file activation-execution.cpp
 * @brief アイテムの発動実行定義
 */

#include "action/activation-execution.h"
#include "action/action-limited.h"
#include "artifact/random-art-effects.h"
#include "artifact/artifact-info.h"
#include "core/window-redrawer.h"
#include "effect/spells-effect-util.h"
#include "floor/geometry.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-floor/monster-generator.h"
#include "monster-floor/place-monster-types.h"
#include "monster-race/monster-race.h"
#include "monster/monster-info.h"
#include "monster/monster-util.h"
#include "object-activation/activation-switcher.h"
#include "object-activation/activation-util.h"
#include "object-enchant/activation-info-table.h"
#include "object-enchant/object-ego.h"
#include "object/object-info.h"
#include "object/object-kind.h"
#include "player-status/player-energy.h"
#include "racial/racial-android.h"
#include "specific-object/monster-ball.h"
#include "spell-kind/spells-launcher.h"
#include "spell-kind/spells-teleport.h"
#include "spell-realm/spells-hex.h"
#include "spell-realm/spells-song.h"
#include "spell/spell-types.h"
#include "sv-definition/sv-lite-types.h"
#include "sv-definition/sv-ring-types.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/monster-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "target/target-getter.h"
#include "term/screen-processor.h"
#include "util/quarks.h"
#include "util/sort.h"
#include "view/display-messages.h"
#include "world/world.h"

static void decide_activation_level(ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->is_fixed_artifact()) {
        ae_ptr->lev = a_info[ae_ptr->o_ptr->name1].level;
        return;
    }

    if (ae_ptr->o_ptr->is_random_artifact()) {
        auto act_ptr = find_activation_info(ae_ptr->o_ptr);
        if (act_ptr.has_value()) {
            ae_ptr->lev = act_ptr.value()->level;
        }

        return;
    }

    if (((ae_ptr->o_ptr->tval == ItemKindType::RING) || (ae_ptr->o_ptr->tval == ItemKindType::AMULET)) && ae_ptr->o_ptr->name2)
        ae_ptr->lev = e_info[ae_ptr->o_ptr->name2].level;
}

static void decide_chance_fail(player_type *player_ptr, ae_type *ae_ptr)
{
    ae_ptr->chance = player_ptr->skill_dev;
    if (player_ptr->confused)
        ae_ptr->chance = ae_ptr->chance / 2;

    ae_ptr->fail = ae_ptr->lev + 5;
    if (ae_ptr->chance > ae_ptr->fail)
        ae_ptr->fail -= (ae_ptr->chance - ae_ptr->fail) * 2;
    else
        ae_ptr->chance -= (ae_ptr->fail - ae_ptr->chance) * 2;

    if (ae_ptr->fail < USE_DEVICE)
        ae_ptr->fail = USE_DEVICE;

    if (ae_ptr->chance < USE_DEVICE)
        ae_ptr->chance = USE_DEVICE;
}

static void decide_activation_success(player_type *player_ptr, ae_type *ae_ptr)
{
    if (player_ptr->pclass == CLASS_BERSERKER) {
        ae_ptr->success = false;
        return;
    }

    if (ae_ptr->chance > ae_ptr->fail) {
        ae_ptr->success = randint0(ae_ptr->chance * 2) >= ae_ptr->fail;
        return;
    }

    ae_ptr->success = randint0(ae_ptr->fail * 2) < ae_ptr->chance;
}

static bool check_activation_success(ae_type *ae_ptr)
{
    if (ae_ptr->success)
        return true;

    if (flush_failure)
        flush();

    msg_print(_("うまく始動させることができなかった。", "You failed to activate it properly."));
    sound(SOUND_FAIL);
    return false;
}

static bool check_activation_conditions(player_type *player_ptr, ae_type *ae_ptr)
{
    if (!check_activation_success(ae_ptr))
        return false;

    if (ae_ptr->o_ptr->timeout) {
        msg_print(_("それは微かに音を立て、輝き、消えた...", "It whines, glows and fades..."));
        return false;
    }

    if (!ae_ptr->o_ptr->xtra4 && (ae_ptr->o_ptr->tval == ItemKindType::FLASK) && ((ae_ptr->o_ptr->sval == SV_LITE_TORCH) || (ae_ptr->o_ptr->sval == SV_LITE_LANTERN))) {
        msg_print(_("燃料がない。", "It has no fuel."));
        PlayerEnergy(player_ptr).reset_player_turn();
        return false;
    }

    return true;
}

/*!
 * @brief アイテムの発動効果を処理する。
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param o_ptr 対象のオブジェクト構造体ポインタ
 * @return 発動実行の是非を返す。
 */
static bool activate_artifact(player_type *player_ptr, object_type *o_ptr)
{
    concptr name = k_info[o_ptr->k_idx].name.c_str();
    auto tmp_act_ptr = find_activation_info(o_ptr);
    if (!tmp_act_ptr.has_value()) {
        msg_print("Activation information is not found.");
        return false;
    }

    auto *act_ptr = tmp_act_ptr.value();
    if (!switch_activation(player_ptr, &o_ptr, act_ptr, name)) {
        return false;
    }

    if (act_ptr->timeout.constant >= 0) {
        o_ptr->timeout = (int16_t)act_ptr->timeout.constant;
        if (act_ptr->timeout.dice > 0)
            o_ptr->timeout += randint1(act_ptr->timeout.dice);

        return true;
    }

    switch (act_ptr->index) {
    case RandomArtActType::BR_FIRE:
        o_ptr->timeout = ((o_ptr->tval == ItemKindType::RING) && (o_ptr->sval == SV_RING_FLAMES)) ? 200 : 250;
        return true;
    case RandomArtActType::BR_COLD:
        o_ptr->timeout = ((o_ptr->tval == ItemKindType::RING) && (o_ptr->sval == SV_RING_ICE)) ? 200 : 250;
        return true;
    case RandomArtActType::TERROR:
        o_ptr->timeout = 3 * (player_ptr->lev + 10);
        return true;
    case RandomArtActType::MURAMASA:
        return true;
    default:
        msg_format("Special timeout is not implemented: %d.", act_ptr->index);
        return false;
    }
}

static bool activate_whistle(player_type *player_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != ItemKindType::WHISTLE)
        return false;

    if (music_singing_any(player_ptr))
        stop_singing(player_ptr);

    if (SpellHex(player_ptr).is_spelling_any()) {
        (void)SpellHex(player_ptr).stop_all_spells();
    }

    std::vector<MONSTER_IDX> who;
    for (MONSTER_IDX pet_ctr = player_ptr->current_floor_ptr->m_max - 1; pet_ctr >= 1; pet_ctr--)
        if (is_pet(&player_ptr->current_floor_ptr->m_list[pet_ctr]) && (player_ptr->riding != pet_ctr))
            who.push_back(pet_ctr);

    uint16_t dummy_why;
    ang_sort(player_ptr, who.data(), &dummy_why, who.size(), ang_sort_comp_pet, ang_sort_swap_hook);
    for (auto pet_ctr : who) {
        teleport_monster_to(player_ptr, pet_ctr, player_ptr->y, player_ptr->x, 100, TELEPORT_PASSIVE);
    }

    ae_ptr->o_ptr->timeout = 100 + randint1(100);
    return true;
}

/*!
 * @brief 装備を発動するコマンドのサブルーチン /
 * Activate a wielded object.  Wielded objects never stack.
 * And even if they did, activatable objects never stack.
 * @param item 発動するオブジェクトの所持品ID
 * @details
 * <pre>
 * Currently, only (some) artifacts, and Dragon Scale Mail, can be activated.
 * But one could, for example, easily make an activatable "Ring of Plasma".
 * Note that it always takes a turn to activate an artifact, even if
 * the user hits "escape" at the "direction" prompt.
 * </pre>
 */
void exe_activate(player_type *player_ptr, INVENTORY_IDX item)
{
    PlayerEnergy(player_ptr).set_player_turn_energy(100);
    ae_type tmp_ae;
    ae_type *ae_ptr = initialize_ae_type(player_ptr, &tmp_ae, item);
    decide_activation_level(ae_ptr);
    decide_chance_fail(player_ptr, ae_ptr);
    if (cmd_limit_time_walk(player_ptr))
        return;

    decide_activation_success(player_ptr, ae_ptr);
    if (!check_activation_conditions(player_ptr, ae_ptr))
        return;

    msg_print(_("始動させた...", "You activate it..."));
    sound(SOUND_ZAP);
    if (activation_index(ae_ptr->o_ptr) > RandomArtActType::NONE) {
        (void)activate_artifact(player_ptr, ae_ptr->o_ptr);
        player_ptr->window_flags |= PW_INVEN | PW_EQUIP;
        return;
    }

    if (activate_whistle(player_ptr, ae_ptr))
        return;

    if (exe_monster_capture(player_ptr, ae_ptr))
        return;

    msg_print(_("おっと、このアイテムは始動できない。", "Oops.  That object cannot be activated."));
}
