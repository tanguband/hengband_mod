﻿#include "mind/mind-blue-mage.h"
#include "action/action-limited.h"
#include "avatar/avatar.h"
#include "blue-magic/blue-magic-caster.h"
#include "blue-magic/learnt-power-getter.h"
#include "core/asking-player.h"
#include "core/player-redraw-types.h"
#include "core/window-redrawer.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-race/race-ability-mask.h"
#include "mspell/monster-power-table.h"
#include "player-status/player-energy.h"
#include "player/player-status-table.h"
#include "realm/realm-types.h"
#include "spell/spell-info.h"
#include "status/bad-status-setter.h"
#include "status/base-status.h"
#include "system/player-type-definition.h"
#include "term/screen-processor.h"
#include "view/display-messages.h"

/*!
 * @brief 青魔法コマンドのメインルーチン /
 * do_cmd_cast calls this function if the player's class is 'Blue-Mage'.
 * @return 処理を実行したらTRUE、キャンセルした場合FALSEを返す。
 */
bool do_cmd_cast_learned(player_type *caster_ptr)
{
    SPELL_IDX n = 0;
    PERCENTAGE chance;
    PERCENTAGE minfail = 0;
    PLAYER_LEVEL plev = caster_ptr->lev;
    monster_power spell;
    bool cast;
    MANA_POINT need_mana;

    if (cmd_limit_confused(caster_ptr))
        return false;

    if (!get_learned_power(caster_ptr, &n))
        return false;

    spell = monster_powers[n];
    need_mana = mod_need_mana(caster_ptr, spell.smana, 0, REALM_NONE);
    if (need_mana > caster_ptr->csp) {
        msg_print(_("ＭＰが足りません。", "You do not have enough mana to use this power."));
        if (!over_exert)
            return false;

        if (!get_check(_("それでも挑戦しますか? ", "Attempt it anyway? ")))
            return false;
    }

    chance = spell.fail;
    if (plev > spell.level)
        chance -= 3 * (plev - spell.level);
    else
        chance += (spell.level - plev);

    chance -= 3 * (adj_mag_stat[caster_ptr->stat_index[A_INT]] - 1);
    chance = mod_spell_chance_1(caster_ptr, chance);
    if (need_mana > caster_ptr->csp) {
        chance += 5 * (need_mana - caster_ptr->csp);
    }

    minfail = adj_mag_fail[caster_ptr->stat_index[A_INT]];
    if (chance < minfail)
        chance = minfail;

    if (caster_ptr->stun > 50)
        chance += 25;
    else if (caster_ptr->stun)
        chance += 15;

    if (chance > 95)
        chance = 95;

    chance = mod_spell_chance_2(caster_ptr, chance);
    const auto spell_type = static_cast<RF_ABILITY>(n);
    if (randint0(100) < chance) {
        if (flush_failure)
            flush();

        msg_print(_("魔法をうまく唱えられなかった。", "You failed to concentrate hard enough!"));
        sound(SOUND_FAIL);
        if (RF_ABILITY_SUMMON_MASK.has(spell_type))
            cast = cast_learned_spell(caster_ptr, spell_type, false);
    } else {
        sound(SOUND_ZAP);
        cast = cast_learned_spell(caster_ptr, spell_type, true);
        if (!cast)
            return false;
    }

    if (need_mana <= caster_ptr->csp) {
        caster_ptr->csp -= need_mana;
    } else {
        int oops = need_mana;
        caster_ptr->csp = 0;
        caster_ptr->csp_frac = 0;
        msg_print(_("精神を集中しすぎて気を失ってしまった！", "You faint from the effort!"));
        (void)set_paralyzed(caster_ptr, caster_ptr->paralyzed + randint1(5 * oops + 1));
        chg_virtue(caster_ptr, V_KNOWLEDGE, -10);
        if (randint0(100) < 50) {
            bool perm = (randint0(100) < 25);
            msg_print(_("体を悪くしてしまった！", "You have damaged your health!"));
            (void)dec_stat(caster_ptr, A_CON, 15 + randint1(10), perm);
        }
    }

    PlayerEnergy(caster_ptr).set_player_turn_energy(100);
    caster_ptr->redraw |= PR_MANA;
    caster_ptr->window_flags |= PW_PLAYER | PW_SPELL;
    return true;
}
