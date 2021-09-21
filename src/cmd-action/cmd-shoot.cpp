﻿#include "cmd-action/cmd-shoot.h"
#include "combat/shoot.h"
#include "floor/floor-object.h"
#include "inventory/inventory-slot-types.h"
#include "mind/snipe-types.h"
#include "object/item-tester-hooker.h"
#include "object/item-use-flags.h"
#include "player/attack-defense-types.h"
#include "player/special-defense-types.h"
#include "spell-kind/spells-teleport.h"
#include "status/action-setter.h"
#include "status/bad-status-setter.h"
#include "sv-definition/sv-bow-types.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "term/screen-processor.h"
#include "timed-effect/player-stun.h"
#include "timed-effect/timed-effects.h"
#include "view/display-messages.h"

/*!
 * @brief 射撃処理のメインルーチン
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param snipe_type ？？？
 * @todo Doxygenの加筆求む
 */
void do_cmd_fire(player_type *player_ptr, SPELL_IDX snipe_type)
{
    OBJECT_IDX item;
    object_type *j_ptr, *ammo_ptr;
    if (player_ptr->wild_mode)
        return;

    player_ptr->is_fired = false;
    j_ptr = &player_ptr->inventory_list[INVEN_BOW];
    if (!j_ptr->tval) {
        msg_print(_("射撃用の武器を持っていない。", "You have nothing to fire with."));
        flush();
        return;
    }

    if (j_ptr->sval == SV_CRIMSON) {
        msg_print(_("この武器は発動して使うもののようだ。", "It's already activated."));
        flush();
        return;
    }

    if (j_ptr->sval == SV_HARP) {
        msg_print(_("この武器で射撃はできない。", "It's not for firing."));
        flush();
        return;
    }

    if (player_ptr->special_defense & KATA_MUSOU)
        set_action(player_ptr, ACTION_NONE);

    concptr q = _("どれを撃ちますか? ", "Fire which item? ");
    concptr s = _("発射されるアイテムがありません。", "You have nothing to fire.");
    ammo_ptr = choose_object(player_ptr, &item, q, s, USE_INVEN | USE_FLOOR, TvalItemTester(player_ptr->tval_ammo));
    if (!ammo_ptr) {
        flush();
        return;
    }

    exe_fire(player_ptr, item, j_ptr, snipe_type);
    if (!player_ptr->is_fired || player_ptr->pclass != CLASS_SNIPER)
        return;

    if (snipe_type == SP_AWAY)
        teleport_player(player_ptr, 10 + (player_ptr->concent * 2), TELEPORT_SPONTANEOUS);

    auto effects = player_ptr->effects();
    if (snipe_type == SP_FINAL) {
        msg_print(_("射撃の反動が体を襲った。", "The weapon's recoil stuns you. "));
        (void)set_slow(player_ptr, player_ptr->slow + randint0(7) + 7, false);
        (void)set_stun(player_ptr, effects->stun()->current() + randint1(25));
    }
}
