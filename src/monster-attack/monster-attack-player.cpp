﻿/*!
 * @brief モンスターからプレイヤーへの直接攻撃処理
 * @date 2020/05/23
 * @author Hourier
 */

#include "monster-attack/monster-attack-player.h"
#include "cmd-action/cmd-attack.h"
#include "cmd-action/cmd-pet.h"
#include "combat/attack-accuracy.h"
#include "combat/attack-criticality.h"
#include "combat/aura-counterattack.h"
#include "combat/combat-options-type.h"
#include "combat/hallucination-attacks-table.h"
#include "core/disturbance.h"
#include "core/player-update-types.h"
#include "dungeon/dungeon-flag-types.h"
#include "dungeon/dungeon.h"
#include "floor/geometry.h"
#include "inventory/inventory-slot-types.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "mind/mind-ninja.h"
#include "mind/mind-samurai.h"
#include "monster-attack/monster-attack-describer.h"
#include "monster-attack/monster-attack-effect.h"
#include "monster-attack/monster-attack-switcher.h"
#include "monster-attack/monster-attack-util.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags3.h"
#include "monster/monster-damage.h"
#include "monster/monster-describer.h"
#include "monster/monster-description-types.h"
#include "monster/monster-info.h"
#include "monster/monster-status.h"
#include "monster/smart-learn-types.h"
#include "object-hook/hook-armor.h"
#include "object/item-tester-hooker.h"
#include "pet/pet-fall-off.h"
#include "player/attack-defense-types.h"
#include "player/player-damage.h"
#include "player/player-skill.h"
#include "player/special-defense-types.h"
#include "spell-realm/spells-hex.h"
#include "status/action-setter.h"
#include "status/bad-status-setter.h"
#include "system/floor-type-definition.h"
#include "system/monster-race-definition.h"
#include "system/monster-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "timed-effect/player-stun.h"
#include "timed-effect/timed-effects.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"

static bool check_no_blow(player_type *player_ptr, monap_type *monap_ptr)
{
    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    if (any_bits(r_ptr->flags1, RF1_NEVER_BLOW)) {
        return false;
    }

    if (d_info[player_ptr->dungeon_idx].flags.has(DF::NO_MELEE)) {
        return false;
    }

    return is_hostile(monap_ptr->m_ptr);
}

/*!
 * @brief プレイヤー死亡等でモンスターからプレイヤーへの直接攻撃処理を途中で打ち切るかどうかを判定する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 * @return 攻撃続行ならばTRUE、打ち切りになったらFALSE
 */
static bool check_monster_continuous_attack(player_type *player_ptr, monap_type *monap_ptr)
{
    if (!monster_is_valid(monap_ptr->m_ptr) || (monap_ptr->method == RBM_NONE)) {
        return false;
    }

    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    if (is_pet(monap_ptr->m_ptr) && (r_ptr->flags1 & RF1_UNIQUE) && (monap_ptr->method == RBM_EXPLODE)) {
        monap_ptr->method = RBM_HIT;
        monap_ptr->d_dice /= 10;
    }

    auto is_neighbor = distance(player_ptr->y, player_ptr->x, monap_ptr->m_ptr->fy, monap_ptr->m_ptr->fx) <= 1;
    return player_ptr->playing && !player_ptr->is_dead && is_neighbor && !player_ptr->leaving;
}

/*!
 * @brief 対邪悪結界が効いている状態で邪悪なモンスターから直接攻撃を受けた時の処理
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 * @return briefに書いた条件＋確率が満たされたらTRUE、それ以外はFALSE
 */
static bool effect_protecion_from_evil(player_type *player_ptr, monap_type *monap_ptr)
{
    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    if ((player_ptr->protevil <= 0) || none_bits(r_ptr->flags3, RF3_EVIL) || (player_ptr->lev < monap_ptr->rlev) || ((randint0(100) + player_ptr->lev) <= 50)) {
        return false;
    }

    if (is_original_ap_and_seen(player_ptr, monap_ptr->m_ptr)) {
        r_ptr->r_flags3 |= RF3_EVIL;
    }

#ifdef JP
    monap_ptr->abbreviate ? msg_format("撃退した。") : msg_format("%^sは撃退された。", monap_ptr->m_name);
    monap_ptr->abbreviate = 1; /* 2回目以降は省略 */
#else
    msg_format("%^s is repelled.", monap_ptr->m_name);
#endif

    return true;
}

static void describe_silly_attacks(monap_type *monap_ptr)
{
    if (monap_ptr->act == nullptr) {
        return;
    }

    if (monap_ptr->do_silly_attack) {
#ifdef JP
        monap_ptr->abbreviate = -1;
#endif
        monap_ptr->act = silly_attacks[randint0(MAX_SILLY_ATTACK)];
    }

#ifdef JP
    if (monap_ptr->abbreviate == 0) {
        msg_format("%^sに%s", monap_ptr->m_name, monap_ptr->act);
    } else if (monap_ptr->abbreviate == 1) {
        msg_format("%s", monap_ptr->act);
    } else {
       /* if (monap_ptr->abbreviate == -1) */
       msg_format("%^s%s", monap_ptr->m_name, monap_ptr->act);
    }

    monap_ptr->abbreviate = 1; /*2回目以降は省略 */
#else
    msg_format("%^s %s%s", monap_ptr->m_name, monap_ptr->act, monap_ptr->do_silly_attack ? " you." : "");
#endif
}

/*!
 * @brief 切り傷と朦朧が同時に発生した時、片方を無効にする
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 */
static void select_cut_stun(monap_type *monap_ptr)
{
    if ((monap_ptr->do_cut == 0) || (monap_ptr->do_stun == 0)) {
        return;
    }

    if (randint0(100) < 50) {
        monap_ptr->do_cut = 0;
    } else {
        monap_ptr->do_stun = 0;
    }
}

static void calc_player_cut(player_type *player_ptr, monap_type *monap_ptr)
{
    if (monap_ptr->do_cut == 0) {
        return;
    }

    auto cut_plus = 0;
    auto criticality = calc_monster_critical(monap_ptr->d_dice, monap_ptr->d_side, monap_ptr->damage);
    switch (criticality) {
    case 0:
        cut_plus = 0;
        break;
    case 1:
        cut_plus = randint1(5);
        break;
    case 2:
        cut_plus = randint1(5) + 5;
        break;
    case 3:
        cut_plus = randint1(20) + 20;
        break;
    case 4:
        cut_plus = randint1(50) + 50;
        break;
    case 5:
        cut_plus = randint1(100) + 100;
        break;
    case 6:
        cut_plus = 300;
        break;
    default:
        cut_plus = 500;
        break;
    }

    if (cut_plus > 0) {
        (void)set_cut(player_ptr, player_ptr->cut + cut_plus);
    }
}

static void calc_player_stun(player_type *player_ptr, monap_type *monap_ptr)
{
    if (monap_ptr->do_stun == 0) {
        return;
    }

    auto stun_plus = 0;
    auto criticality = calc_monster_critical(monap_ptr->d_dice, monap_ptr->d_side, monap_ptr->damage);
    switch (criticality) {
    case 0:
        stun_plus = 0;
        break;
    case 1:
        stun_plus = randint1(5);
        break;
    case 2:
        stun_plus = randint1(5) + 10;
        break;
    case 3:
        stun_plus = randint1(10) + 20;
        break;
    case 4:
        stun_plus = randint1(15) + 30;
        break;
    case 5:
        stun_plus = randint1(20) + 40;
        break;
    case 6:
        stun_plus = 80;
        break;
    default:
        stun_plus = 150;
        break;
    }

    if (stun_plus > 0) {
        (void)set_stun(player_ptr, player_ptr->effects()->stun()->current() + stun_plus);
    }
}

static void monster_explode(player_type *player_ptr, monap_type *monap_ptr)
{
    if (!monap_ptr->explode) {
        return;
    }

    sound(SOUND_EXPLODE);
    MonsterDamageProcessor mdp(player_ptr, monap_ptr->m_idx, monap_ptr->m_ptr->hp + 1, &monap_ptr->fear);
    if (mdp.mon_take_hit(nullptr)) {
        monap_ptr->blinked = false;
        monap_ptr->alive = false;
    }
}

static void describe_attack_evasion(player_type *player_ptr, monap_type *monap_ptr)
{
    if (!monap_ptr->m_ptr->ml) {
        return;
    }

    disturb(player_ptr, true, true);
#ifdef JP
    auto is_suiken = any_bits(player_ptr->special_attack, ATTACK_SUIKEN);
    if (monap_ptr->abbreviate) {
        msg_format("%sかわした。", is_suiken ? "奇妙な動きで" : "");
    } else {
        msg_format("%s%^sの攻撃をかわした。", is_suiken ? "奇妙な動きで" : "", monap_ptr->m_name);
    }

    monap_ptr->abbreviate = 1; /* 2回目以降は省略 */
#else
    msg_format("%^s misses you.", monap_ptr->m_name);
#endif
}

static void gain_armor_exp(player_type *player_ptr, monap_type *monap_ptr)
{
    const auto o_ptr_mh = &player_ptr->inventory_list[INVEN_MAIN_HAND];
    const auto o_ptr_sh = &player_ptr->inventory_list[INVEN_SUB_HAND];
    if (!o_ptr_mh->is_armour() && !o_ptr_sh->is_armour()) {
        return;
    }

    auto cur = player_ptr->skill_exp[SKILL_SHIELD];
    auto max = s_info[player_ptr->pclass].s_max[SKILL_SHIELD];
    if (cur >= max) {
        return;
    }

    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    auto target_level = r_ptr->level;
    auto increment = 0;
    if ((cur / 100) < target_level) {
        auto addition = (cur / 100 + 15) < target_level ? (target_level - (cur / 100 + 15)) : 0;
        increment += 1 + addition;
    }

    player_ptr->skill_exp[SKILL_SHIELD] = MIN(max, cur + increment);
    player_ptr->update |= (PU_BONUS);
}

/*!
 * @brief モンスターから直接攻撃を1回受けた時の処理
 * @return 対邪悪結界により攻撃が当たらなかったらFALSE、それ以外はTRUE
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 * @details 最大4 回/モンスター/ターン、このルーチンを通る
 */
static bool process_monster_attack_hit(player_type *player_ptr, monap_type *monap_ptr)
{
    disturb(player_ptr, true, true);
    if (effect_protecion_from_evil(player_ptr, monap_ptr)) {
        return false;
    }

    monap_ptr->do_cut = 0;
    monap_ptr->do_stun = 0;
    describe_monster_attack_method(monap_ptr);
    describe_silly_attacks(monap_ptr);
    monap_ptr->obvious = true;
    monap_ptr->damage = damroll(monap_ptr->d_dice, monap_ptr->d_side);
    if (monap_ptr->explode) {
        monap_ptr->damage = 0;
    }

    switch_monster_blow_to_player(player_ptr, monap_ptr);
    select_cut_stun(monap_ptr);
    calc_player_cut(player_ptr, monap_ptr);
    calc_player_stun(player_ptr, monap_ptr);
    monster_explode(player_ptr, monap_ptr);
    process_aura_counterattack(player_ptr, monap_ptr);
    return true;
}

/*!
 * @brief 一部の打撃種別の場合のみ、避けた旨のメッセージ表示と盾技能スキル向上を行う
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 */
static void process_monster_attack_evasion(player_type *player_ptr, monap_type *monap_ptr)
{
    switch (monap_ptr->method) {
    case RBM_HIT:
    case RBM_TOUCH:
    case RBM_PUNCH:
    case RBM_KICK:
    case RBM_CLAW:
    case RBM_BITE:
    case RBM_STING:
    case RBM_SLASH:
    case RBM_BUTT:
    case RBM_CRUSH:
    case RBM_ENGULF:
    case RBM_CHARGE:
        describe_attack_evasion(player_ptr, monap_ptr);
        gain_armor_exp(player_ptr, monap_ptr);
        monap_ptr->damage = 0;
        return;
    default:
        return;
    }
}

/*!
 * @brief モンスターの打撃情報を蓄積させる
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param monap_ptr モンスターからプレイヤーへの直接攻撃構造体への参照ポインタ
 * @param ap_cnt モンスターの打撃 N回目
 * @details どの敵が何をしてきたか正しく認識できていなければ情報を蓄積しない.
 * 非自明な類の打撃については、そのダメージが 0 ならば基本的に知識が増えない.
 * 但し、既に一定以上の知識があれば常に知識が増える(何をされたのか察知できる).
 */
static void increase_blow_type_seen(player_type *player_ptr, monap_type *monap_ptr, const int ap_cnt)
{
    if (!is_original_ap_and_seen(player_ptr, monap_ptr->m_ptr) || monap_ptr->do_silly_attack) {
        return;
    }

    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    if (!monap_ptr->obvious && (monap_ptr->damage == 0) && (r_ptr->r_blows[ap_cnt] <= 10)) {
        return;
    }

    if (r_ptr->r_blows[ap_cnt] < MAX_UCHAR) {
        r_ptr->r_blows[ap_cnt]++;
    }
}

/*!
 * @brief モンスターからプレイヤーへの打撃処理本体
 * @return 打撃に反応してプレイヤーがその場から離脱したかどうか
 */
static bool process_monster_blows(player_type *player_ptr, monap_type *monap_ptr)
{
    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    for (auto ap_cnt = 0; ap_cnt < MAX_NUM_BLOWS; ap_cnt++) {
        monap_ptr->obvious = false;
        monap_ptr->damage = 0;
        monap_ptr->act = nullptr;
        monap_ptr->effect = r_ptr->blow[ap_cnt].effect;
        monap_ptr->method = r_ptr->blow[ap_cnt].method;
        monap_ptr->d_dice = r_ptr->blow[ap_cnt].d_dice;
        monap_ptr->d_side = r_ptr->blow[ap_cnt].d_side;

        if (!check_monster_continuous_attack(player_ptr, monap_ptr)) {
            break;
        }

        // effect が RBE_NONE (無効値)になることはあり得ないはずだが、万一そう
        // なっていたら単に攻撃を打ち切る。
        // r_info.txt の "B:" トークンに effect 以降を書き忘れた場合が該当する。
        if (monap_ptr->effect == RBE_NONE) {
            plog("unexpected: monap_ptr->effect == RBE_NONE");
            break;
        }

        if (monap_ptr->method == RBM_SHOOT) {
            continue;
        }

        // フレーバーの打撃は必中扱い。それ以外は通常の命中判定を行う。
        monap_ptr->ac = player_ptr->ac + player_ptr->to_a;
        bool hit;
        if (monap_ptr->effect == RBE_FLAVOR) {
            hit = true;
        } else {
            const int power = mbe_info[monap_ptr->effect].power;
            hit = check_hit_from_monster_to_player(player_ptr, power, monap_ptr->rlev, monster_stunned_remaining(monap_ptr->m_ptr));
        }

        if (hit) {
            // 命中した。命中処理と思い出処理を行う。
            // 打撃そのものは対邪悪結界で撃退した可能性がある。
            const bool protect = !process_monster_attack_hit(player_ptr, monap_ptr);
            increase_blow_type_seen(player_ptr, monap_ptr, ap_cnt);

            // 撃退成功時はそのまま次の打撃へ移行。
            if (protect)
                continue;

            // 撃退失敗時は落馬処理、変わり身のテレポート処理を行う。
            check_fall_off_horse(player_ptr, monap_ptr);
            if (player_ptr->special_defense & NINJA_KAWARIMI) {
                // 変わり身のテレポートが成功したら攻撃を打ち切り、プレイヤーが離脱した旨を返す。
                if (kawarimi(player_ptr, false))
                    return true;
            }
        } else {
            // 命中しなかった。回避時の処理、思い出処理を行う。
            process_monster_attack_evasion(player_ptr, monap_ptr);
            increase_blow_type_seen(player_ptr, monap_ptr, ap_cnt);
        }
    }

    // 通常はプレイヤーはその場にとどまる。
    return false;
}

static void postprocess_monster_blows(player_type *player_ptr, monap_type *monap_ptr)
{
    SpellHex spell_hex(player_ptr, monap_ptr);
    spell_hex.store_vengeful_damage(monap_ptr->get_damage);
    spell_hex.eyes_on_eyes();
    musou_counterattack(player_ptr, monap_ptr);
    spell_hex.thief_teleport();
    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    if (player_ptr->is_dead && (r_ptr->r_deaths < MAX_SHORT) && !player_ptr->current_floor_ptr->inside_arena) {
        r_ptr->r_deaths++;
    }

    if (monap_ptr->m_ptr->ml && monap_ptr->fear && monap_ptr->alive && !player_ptr->is_dead) {
        sound(SOUND_FLEE);
        msg_format(_("%^sは恐怖で逃げ出した！", "%^s flees in terror!"), monap_ptr->m_name);
    }

    if (player_ptr->special_defense & KATA_IAI) {
        set_action(player_ptr, ACTION_NONE);
    }
}

/*!
 * @brief モンスターからプレイヤーへの打撃処理 / Attack the player via physical attacks.
 * @param m_idx 打撃を行うモンスターのID
 * @return 実際に攻撃処理を行った場合TRUEを返す
 */
bool make_attack_normal(player_type *player_ptr, MONSTER_IDX m_idx)
{
    monap_type tmp_monap;
    monap_type *monap_ptr = initialize_monap_type(player_ptr, &tmp_monap, m_idx);
    if (!check_no_blow(player_ptr, monap_ptr)) {
        return false;
    }

    auto *r_ptr = &r_info[monap_ptr->m_ptr->r_idx];
    monap_ptr->rlev = ((r_ptr->level >= 1) ? r_ptr->level : 1);
    monster_desc(player_ptr, monap_ptr->m_name, monap_ptr->m_ptr, 0);
    monster_desc(player_ptr, monap_ptr->ddesc, monap_ptr->m_ptr, MD_WRONGDOER_NAME);
    if (any_bits(player_ptr->special_defense, KATA_IAI)) {
        msg_format(_("相手が襲いかかる前に素早く武器を振るった。", "You took sen, drew and cut in one motion before %s moved."), monap_ptr->m_name);
        if (do_cmd_attack(player_ptr, monap_ptr->m_ptr->fy, monap_ptr->m_ptr->fx, HISSATSU_IAI)) {
            return true;
        }
    }

    auto is_kawarimi = any_bits(player_ptr->special_defense, NINJA_KAWARIMI);
    auto can_activate_kawarimi = randint0(55) < (player_ptr->lev * 3 / 5 + 20);
    if (is_kawarimi && can_activate_kawarimi && kawarimi(player_ptr, true)) {
        return true;
    }

    monap_ptr->blinked = false;
    if (process_monster_blows(player_ptr, monap_ptr)) {
        return true;
    }

    postprocess_monster_blows(player_ptr, monap_ptr);
    return true;
}
