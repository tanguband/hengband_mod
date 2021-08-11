﻿#include "cmd-action/cmd-open-close.h"
#include "action/open-close-execution.h"
#include "action/open-util.h"
#include "cmd-action/cmd-attack.h"
#include "core/disturbance.h"
#include "core/player-redraw-types.h"
#include "floor/geometry.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "inventory/inventory-object.h"
#include "inventory/inventory-slot-types.h"
#include "io/input-key-requester.h"
#include "player-status/player-energy.h"
#include "player/attack-defense-types.h"
#include "player/special-defense-types.h"
#include "specific-object/chest.h"
#include "status/action-setter.h"
#include "status/experience.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "target/target-getter.h"
#include "term/screen-processor.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"

/*!
 * @brief 箱を開ける実行処理 /
 * Attempt to open the given chest at the given location
 * @param y 箱の存在するマスのY座標
 * @param x 箱の存在するマスのX座標
 * @param o_idx 箱のオブジェクトID
 * @return 箱が開かなかった場合TRUE / Returns TRUE if repeated commands may continue
 * @details
 * Assume there is no monster blocking the destination
 */
static bool exe_open_chest(player_type *creature_ptr, POSITION y, POSITION x, OBJECT_IDX o_idx)
{
    bool flag = true;
    bool more = false;
    object_type *o_ptr = &creature_ptr->current_floor_ptr->o_list[o_idx];
    PlayerEnergy(creature_ptr).set_player_turn_energy(100);
    if (o_ptr->pval > 0) {
        flag = false;
        int i = creature_ptr->skill_dis;
        if (creature_ptr->blind || no_lite(creature_ptr))
            i = i / 10;

        if (creature_ptr->confused || creature_ptr->image)
            i = i / 10;

        int j = i - o_ptr->pval;
        if (j < 2)
            j = 2;

        if (randint0(100) < j) {
            msg_print(_("鍵をはずした。", "You have picked the lock."));
            gain_exp(creature_ptr, 1);
            flag = true;
        } else {
            more = true;
            if (flush_failure)
                flush();

            msg_print(_("鍵をはずせなかった。", "You failed to pick the lock."));
        }
    }

    if (flag) {
        chest_trap(creature_ptr, y, x, o_idx);
        chest_death(creature_ptr, false, y, x, o_idx);
    }

    return more;
}

/*!
 * @brief 「開ける」コマンドのメインルーチン /
 * Open a closed/locked/jammed door or a closed/locked chest.
 * @details
 * Unlocking a locked door/chest is worth one experience point.
 */
void do_cmd_open(player_type *creature_ptr)
{
    POSITION y, x;
    DIRECTION dir;
    OBJECT_IDX o_idx;
    bool more = false;
    if (creature_ptr->wild_mode)
        return;

    if (creature_ptr->special_defense & KATA_MUSOU)
        set_action(creature_ptr, ACTION_NONE);

    if (easy_open) {
        int num_doors = count_dt(creature_ptr, &y, &x, is_closed_door, false);
        int num_chests = count_chests(creature_ptr, &y, &x, false);
        if (num_doors || num_chests) {
            bool too_many = (num_doors && num_chests) || (num_doors > 1) || (num_chests > 1);
            if (!too_many)
                command_dir = coords_to_dir(creature_ptr, y, x);
        }
    }

    if (command_arg) {
        command_rep = command_arg - 1;
        creature_ptr->redraw |= PR_STATE;
        command_arg = 0;
    }

    if (get_rep_dir(creature_ptr, &dir, true)) {
        FEAT_IDX feat;
        grid_type *g_ptr;
        y = creature_ptr->y + ddy[dir];
        x = creature_ptr->x + ddx[dir];
        g_ptr = &creature_ptr->current_floor_ptr->grid_array[y][x];
        feat = g_ptr->get_feat_mimic();
        o_idx = chest_check(creature_ptr->current_floor_ptr, y, x, false);
        if (!has_flag(f_info[feat].flags, FF_OPEN) && !o_idx) {
            msg_print(_("そこには開けるものが見当たらない。", "You see nothing there to open."));
        } else if (g_ptr->m_idx && creature_ptr->riding != g_ptr->m_idx) {
            PlayerEnergy(creature_ptr).set_player_turn_energy(100);
            msg_print(_("モンスターが立ちふさがっている！", "There is a monster in the way!"));
            do_cmd_attack(creature_ptr, y, x, HISSATSU_NONE);
        } else if (o_idx) {
            more = exe_open_chest(creature_ptr, y, x, o_idx);
        } else {
            more = exe_open(creature_ptr, y, x);
        }
    }

    if (!more)
        disturb(creature_ptr, false, false);
}

/*!
 * @brief 「閉じる」コマンドのメインルーチン /
 * Close an open door.
 * @details
 * Unlocking a locked door/chest is worth one experience point.
 */
void do_cmd_close(player_type *creature_ptr)
{
    POSITION y, x;
    DIRECTION dir;
    bool more = false;
    if (creature_ptr->wild_mode)
        return;

    if (creature_ptr->special_defense & KATA_MUSOU)
        set_action(creature_ptr, ACTION_NONE);

    if (easy_open && (count_dt(creature_ptr, &y, &x, is_open, false) == 1))
        command_dir = coords_to_dir(creature_ptr, y, x);

    if (command_arg) {
        command_rep = command_arg - 1;
        creature_ptr->redraw |= (PR_STATE);
        command_arg = 0;
    }

    if (get_rep_dir(creature_ptr, &dir, false)) {
        grid_type *g_ptr;
        FEAT_IDX feat;
        y = creature_ptr->y + ddy[dir];
        x = creature_ptr->x + ddx[dir];
        g_ptr = &creature_ptr->current_floor_ptr->grid_array[y][x];
        feat = g_ptr->get_feat_mimic();
        if (!has_flag(f_info[feat].flags, FF_CLOSE)) {
            msg_print(_("そこには閉じるものが見当たらない。", "You see nothing there to close."));
        } else if (g_ptr->m_idx) {
            PlayerEnergy(creature_ptr).set_player_turn_energy(100);
            msg_print(_("モンスターが立ちふさがっている！", "There is a monster in the way!"));
            do_cmd_attack(creature_ptr, y, x, HISSATSU_NONE);
        } else {
            more = exe_close(creature_ptr, y, x);
        }
    }

    if (!more)
        disturb(creature_ptr, false, false);
}

/*!
 * @brief 箱、床のトラップ解除処理双方の統合メインルーチン /
 * Disarms a trap, or chest
 */
void do_cmd_disarm(player_type *creature_ptr)
{
    POSITION y, x;
    DIRECTION dir;
    OBJECT_IDX o_idx;
    bool more = false;
    if (creature_ptr->wild_mode)
        return;

    if (creature_ptr->special_defense & KATA_MUSOU)
        set_action(creature_ptr, ACTION_NONE);

    if (easy_disarm) {
        int num_traps = count_dt(creature_ptr, &y, &x, is_trap, true);
        int num_chests = count_chests(creature_ptr, &y, &x, true);
        if (num_traps || num_chests) {
            bool too_many = (num_traps && num_chests) || (num_traps > 1) || (num_chests > 1);
            if (!too_many)
                command_dir = coords_to_dir(creature_ptr, y, x);
        }
    }

    if (command_arg) {
        command_rep = command_arg - 1;
        creature_ptr->redraw |= (PR_STATE);
        command_arg = 0;
    }

    if (get_rep_dir(creature_ptr, &dir, true)) {
        grid_type *g_ptr;
        FEAT_IDX feat;
        y = creature_ptr->y + ddy[dir];
        x = creature_ptr->x + ddx[dir];
        g_ptr = &creature_ptr->current_floor_ptr->grid_array[y][x];
        feat = g_ptr->get_feat_mimic();
        o_idx = chest_check(creature_ptr->current_floor_ptr, y, x, true);
        if (!is_trap(creature_ptr, feat) && !o_idx) {
            msg_print(_("そこには解除するものが見当たらない。", "You see nothing there to disarm."));
        } else if (g_ptr->m_idx && creature_ptr->riding != g_ptr->m_idx) {
            msg_print(_("モンスターが立ちふさがっている！", "There is a monster in the way!"));
            do_cmd_attack(creature_ptr, y, x, HISSATSU_NONE);
        } else if (o_idx) {
            more = exe_disarm_chest(creature_ptr, y, x, o_idx);
        } else {
            more = exe_disarm(creature_ptr, y, x, dir);
        }
    }

    if (!more)
        disturb(creature_ptr, false, false);
}

/*!
 * @brief 「打ち破る」動作コマンドのメインルーチン /
 * Bash open a door, success based on character strength
 * @details
 * <pre>
 * For a closed door, pval is positive if locked; negative if stuck.
 *
 * For an open door, pval is positive for a broken door.
 *
 * A closed door can be opened - harder if locked. Any door might be
 * bashed open (and thereby broken). Bashing a door is (potentially)
 * faster! You move into the door way. To open a stuck door, it must
 * be bashed. A closed door can be jammed (see do_cmd_spike()).
 *
 * Creatures can also open or bash doors, see elsewhere.
 * </pre>
 */
void do_cmd_bash(player_type *creature_ptr)
{
    POSITION y, x;
    DIRECTION dir;
    grid_type *g_ptr;
    bool more = false;
    if (creature_ptr->wild_mode)
        return;

    if (creature_ptr->special_defense & KATA_MUSOU)
        set_action(creature_ptr, ACTION_NONE);

    if (command_arg) {
        command_rep = command_arg - 1;
        creature_ptr->redraw |= (PR_STATE);
        command_arg = 0;
    }

    if (get_rep_dir(creature_ptr, &dir, false)) {
        FEAT_IDX feat;
        y = creature_ptr->y + ddy[dir];
        x = creature_ptr->x + ddx[dir];
        g_ptr = &creature_ptr->current_floor_ptr->grid_array[y][x];
        feat = g_ptr->get_feat_mimic();
        if (!has_flag(f_info[feat].flags, FF_BASH)) {
            msg_print(_("そこには体当たりするものが見当たらない。", "You see nothing there to bash."));
        } else if (g_ptr->m_idx) {
            PlayerEnergy(creature_ptr).set_player_turn_energy(100);
            msg_print(_("モンスターが立ちふさがっている！", "There is a monster in the way!"));
            do_cmd_attack(creature_ptr, y, x, HISSATSU_NONE);
        } else {
            more = exe_bash(creature_ptr, y, x, dir);
        }
    }

    if (!more)
        disturb(creature_ptr, false, false);
}


/*!
 * @brief 「くさびを打つ」ために必要なオブジェクトを所持しているかどうかの判定を返す /
 * Find the index of some "spikes", if possible.
 * @param ip くさびとして打てるオブジェクトのID
 * @return オブジェクトがある場合TRUEを返す
 * @details
 * <pre>
 * Let user choose a pile of spikes, perhaps?
 * </pre>
 */
static bool get_spike(player_type *creature_ptr, INVENTORY_IDX *ip)
{
    for (INVENTORY_IDX i = 0; i < INVEN_PACK; i++) {
        object_type *o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;

        if (o_ptr->tval == TV_SPIKE) {
            *ip = i;
            return true;
        }
    }

    return false;
}

/*!
 * @brief 「くさびを打つ」動作コマンドのメインルーチン /
 * Jam a closed door with a spike
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @details
 * <pre>
 * This command may NOT be repeated
 * </pre>
 */
void do_cmd_spike(player_type *creature_ptr)
{
    DIRECTION dir;
    if (creature_ptr->wild_mode)
        return;

    if (creature_ptr->special_defense & KATA_MUSOU)
        set_action(creature_ptr, ACTION_NONE);

    if (!get_rep_dir(creature_ptr, &dir, false))
        return;

    POSITION y = creature_ptr->y + ddy[dir];
    POSITION x = creature_ptr->x + ddx[dir];
    grid_type *g_ptr;
    g_ptr = &creature_ptr->current_floor_ptr->grid_array[y][x];
    FEAT_IDX feat = g_ptr->get_feat_mimic();
    INVENTORY_IDX item;
    if (!has_flag(f_info[feat].flags, FF_SPIKE)) {
        msg_print(_("そこにはくさびを打てるものが見当たらない。", "You see nothing there to spike."));
    } else if (!get_spike(creature_ptr, &item)) {
        msg_print(_("くさびを持っていない！", "You have no spikes!"));
    } else if (g_ptr->m_idx) {
        PlayerEnergy(creature_ptr).set_player_turn_energy(100);
        msg_print(_("モンスターが立ちふさがっている！", "There is a monster in the way!"));
        do_cmd_attack(creature_ptr, y, x, HISSATSU_NONE);
    } else {
        PlayerEnergy(creature_ptr).set_player_turn_energy(100);
        msg_format(_("%sにくさびを打ち込んだ。", "You jam the %s with a spike."), f_info[feat].name.c_str());
        cave_alter_feat(creature_ptr, y, x, FF_SPIKE);
        vary_item(creature_ptr, item, -1);
    }
}
