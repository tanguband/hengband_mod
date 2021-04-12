﻿/*!
 * @brief 元素使いの魔法系統
 */

#include "mind-elementalist.h"
#include "action/action-limited.h"
#include "cmd-action/cmd-spell.h"
#include "cmd-action/cmd-mind.h"
#include "cmd-io/cmd-gameoption.h"
#include "core/asking-player.h"
#include "core/hp-mp-processor.h"
#include "core/player-redraw-types.h"
#include "core/stuff-handler.h"
#include "core/window-redrawer.h"
#include "effect/effect-characteristics.h"
#include "effect/effect-monster-util.h"
#include "effect/effect-processor.h"
#include "effect/spells-effect-util.h"
#include "floor/cave.h"
#include "floor/floor-util.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "game-option/text-display-options.h"
#include "grid/feature-flag-types.h"
#include "io/command-repeater.h"
#include "io/input-key-acceptor.h"
#include "io/input-key-requester.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "mind/mind-explanations-table.h"
#include "monster/monster-describer.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags3.h"
#include "monster-race/race-flags7.h"
#include "monster-race/race-flags-resistance.h"
#include "mind/mind-mindcrafter.h"
#include "player/player-status-table.h"
#include "player-info/avatar.h"
#include "player-status/player-status-base.h"
#include "racial/racial-util.h"
#include "spell-kind/earthquake.h"
#include "spell-kind/spells-charm.h"
#include "spell-kind/spells-detection.h"
#include "spell-kind/spells-genocide.h"
#include "spell-kind/spells-launcher.h"
#include "spell-kind/spells-lite.h"
#include "spell-kind/magic-item-recharger.h"
#include "spell-kind/spells-beam.h"
#include "spell-kind/spells-sight.h"
#include "spell-kind/spells-teleport.h"
#include "spell-kind/spells-world.h"
#include "status/bad-status-setter.h"
#include "status/base-status.h"
#include "system/floor-type-definition.h"
#include "system/game-option-types.h"
#include "util/bit-flags-calculator.h"
#include "util/buffer-shaper.h"
#include "util/int-char-converter.h"
#include "target/grid-selector.h"
#include "target/target-getter.h"
#include "term/screen-processor.h"
#include "term/term-color-types.h"
#include "view/display-messages.h"
#include <string>
#include <array>
#include <unordered_map>

/*!
 * @brief 元素魔法呪文のID定義
 */
enum class ElementSpells {
    BOLT_1ST = 0,
    MON_DETECT = 1,
    PERCEPT = 2,
    CURE = 3,
    BOLT_2ND = 4,
    MAG_DETECT = 5,
    BALL_3RD = 6,
    BALL_1ST = 7,
    BREATH_2ND = 8,
    ANNIHILATE = 9,
    BOLT_3RD = 10,
    WAVE_1ST = 11,
    BALL_2ND = 12,
    BURST_1ST = 13,
    STORM_2ND = 14,
    BREATH_1ST = 15,
    STORM_3ND = 16,
    MAX
};

/*!
 * @brief 元素魔法タイプ構造体
 */
struct element_type {
    std::string_view title; //!< 領域名
    std::array<spells_type, 3> type; //!< 属性タイプリスト
    std::array<std::string_view, 3> name; //!< 属性名リスト
    std::unordered_map<spells_type, spells_type> extra; //!< 追加属性タイプ
};

/*!
 * @brief 元素魔法難易度構造体
 */
struct element_power {
    int elem; //!< 使用属性番号
    mind_type info; //!< 難易度構造体
};

using element_type_list = const std::unordered_map<ElementRealm, element_type>;
using element_power_list = const std::unordered_map<ElementSpells, element_power>;
using element_tip_list = const std::unordered_map<ElementSpells, std::string_view>;
using element_text_list = const std::unordered_map<ElementRealm, std::string_view>;

// clang-format off
/*!
 * @brief 元素魔法タイプ定義
 */
static element_type_list element_types = {
    {
        ElementRealm::FIRE, {
            _("炎", "Fire"),
            { GF_FIRE, GF_HELL_FIRE, GF_PLASMA },
            { _("火炎", "Fire"), _("業火", "Hell Fire"), _("プラズマ", "Plasma") },
            { },
        }
    },
    {
        ElementRealm::ICE, {
            _("氷", "Ice"),
            { GF_COLD, GF_INERTIAL, GF_TIME },
            { _("冷気", "Ice"), _("遅鈍", "Inertia"), _("時間逆転", "Time Stream") },
            { { GF_COLD, GF_ICE} },
        }
    },
    {
        ElementRealm::SKY, {
            _("空", "Sky"),
            { GF_ELEC, GF_LITE, GF_MANA },
            { _("電撃", "Lightning"), _("光", "Light"), _("魔力", "Mana") },
            { },
        }
    },
    {
        ElementRealm::SEA, {
            _("海", "Sea"),
            { GF_ACID, GF_WATER, GF_DISINTEGRATE },
            { _("酸", "Acid"), _("水", "Water"), _("分解", "Disintegration") },
            { },
        }
    },
    {
        ElementRealm::DARKNESS, {
            _("闇", "Darkness"),
            { GF_DARK, GF_NETHER, GF_VOID },
            { _("暗黒", "Darkness"), _("地獄", "Nether"), _("虚無", "void") },
            { { GF_DARK, GF_ABYSS } },
        }
    },
    {
        ElementRealm::CHAOS, {
            _("混沌", "Chaos"),
            { GF_CONFUSION, GF_CHAOS, GF_NEXUS },
            { _("混乱", "Confusion"), _("カオス", "Chaos"), _("因果混乱", "Nexus") },
            { },
        }
    },
    {
        ElementRealm::EARTH, {
            _("地", "Earth"),
            { GF_SHARDS, GF_FORCE, GF_METEOR },
            { _("破片", "Shards"), _("フォース", "Force"), _("隕石", "Meteor") },
            { },
        }
    },
    {
        ElementRealm::DEATH, {
            _("瘴気", "Death"),
            { GF_POIS, GF_HYPODYNAMIA, GF_DISENCHANT },
            { _("毒", "Poison"), _("吸血", "Drain Life"), _("劣化", "Disenchantment") },
            { },
        }
    },
};

/*!
 * @brief 元素魔法呪文定義
 */
static element_power_list element_powers = {
    { ElementSpells::BOLT_1ST,   { 0, {  1,  1,  15, _("%sの矢", "%s Bolt") }}},
    { ElementSpells::MON_DETECT, { 0, {  2,  2,  20, _("モンスター感知", "Detect Monsters") }}},
    { ElementSpells::PERCEPT,    { 0, {  5,  5,  50, _("擬似鑑定", "Psychometry") }}},
    { ElementSpells::CURE,       { 0, {  6,  5,  35, _("傷の治癒", "Cure Wounds") }}},
    { ElementSpells::BOLT_2ND,   { 1, {  8,  6,  35, _("%sの矢", "%s Bolt") }}},
    { ElementSpells::MAG_DETECT, { 0, { 10,  8,  60, _("魔法感知", "Detect Magical Objs") }}},
    { ElementSpells::BALL_3RD,   { 2, { 15, 10,  55, _("%s放射", "%s Spout") }}},
    { ElementSpells::BALL_1ST,   { 0, { 18, 13,  65, _("%sの球", "%s Ball") }}},
    { ElementSpells::BREATH_2ND, { 1, { 21, 20,  70, _("%sのブレス", "Breath of %s") }}},
    { ElementSpells::ANNIHILATE, { 0, { 24, 20,  75, _("モンスター消滅", "Annihilation") }}},
    { ElementSpells::BOLT_3RD,   { 2, { 25, 15,  60, _("%sの矢", "%s Bolt") }}},
    { ElementSpells::WAVE_1ST,   { 0, { 28, 30,  75, _("元素の波動", "Elemental Wave") }}},
    { ElementSpells::BALL_2ND,   { 1, { 28, 22,  75, _("%sの球", "%s Ball") }}},
    { ElementSpells::BURST_1ST,  { 0, { 33, 35,  75, _("精気乱射", "%s Blast") }}},
    { ElementSpells::STORM_2ND,  { 1, { 35, 30,  75, _("%sの嵐", "%s Storm") }}},
    { ElementSpells::BREATH_1ST, { 0, { 42, 48,  75, _("%sのブレス", "Breath of %s") }}},
    { ElementSpells::STORM_3ND,  { 2, { 45, 60,  80, _("%sの嵐", "%s Storm") }}},
};

/*!
 * @brief 元素魔法呪文説明文定義
 */
static element_tip_list element_tips = {
    { ElementSpells::BOLT_1ST,
    _("弱い%sの矢を放つ。", "Fire a weak bolt of %s.") },
    { ElementSpells::MON_DETECT,
    _("近くの全てのモンスターを感知する。", "Detects monsters.") },
    { ElementSpells::PERCEPT,
    _("アイテムの雰囲気を知る。", "Gives feeling of an item.") },
    { ElementSpells::CURE,
    _("怪我と体力を少し回復させる。", "Heals HP and wounds a bit.") },
    { ElementSpells::BOLT_2ND,
    _("%sの矢を放つ。", "Fire a bolt of %s.") },
    { ElementSpells::MAG_DETECT,
    _("近くの魔法のアイテムを感知する。", "Detects magic devices.") },
    { ElementSpells::BALL_3RD,
    _("高威力で射程が短い%sの球を放つ。", "Fire a strong, short-range, ball of %s.") },
    { ElementSpells::BALL_1ST,
    _("%sの球を放つ。",  "Fire a ball of %s.") },
    { ElementSpells::BREATH_2ND,
    _("%sのブレスを吐く。", "Fire a breath of %s.") },
    { ElementSpells::ANNIHILATE,
    _("%s耐性のないモンスターを1体抹殺する。", "Erase a monster unless it resists %s.") },
    { ElementSpells::BOLT_3RD,
    _("%sの矢を放つ。", "Fire a bolt of %s.") },
    { ElementSpells::WAVE_1ST,
    _("視界内の全ての敵に%sによるダメージを与える。", "Inflict %s damage on all monsters in sight.") },
    { ElementSpells::BALL_2ND,
    _("%sの球を放つ。",  "Fire a ball of %s.") },
    { ElementSpells::BURST_1ST,
    _("ランダムな方向に%sの矢を放つ。", "Fire some bolts of %s in random direction.") },
    { ElementSpells::STORM_2ND,
    _("%sの巨大な球を放つ。", "Fire a large ball of %s.") },
    { ElementSpells::BREATH_1ST,
    _("%sのブレスを吐く。", "Fire a breath of %s.") },
    { ElementSpells::STORM_3ND,
    _("%sの巨大な球を放つ。", "Fire a large ball of %s.") },
};

/*!
 * @brief 元素魔法選択時説明文定義
 */
static element_text_list element_texts = {
    { ElementRealm::FIRE,
    _("炎系統は巨大なエネルギーで灼熱を生み出し、全ての敵を燃やし尽くそうとします。",
        "Great energy of Fire system will be able to burn out all of your enemies.")},
    { ElementRealm::ICE,
    _("氷系統の魔法はその冷たさで敵の動きを奪い尽くし、魂すらも止めてしまうでしょう。",
        "Ice system will freeze your enemies, even their souls.")},
    { ElementRealm::SKY,
    _("空系統は大いなる天空のエネルギーを駆使して敵の全てを撃滅できます。",
        "Sky system can terminate all of your enemies powerfully with the energy of the great sky.")},
    { ElementRealm::SEA,
    _("海系統はその敵の全てを溶かし、大いなる海へと返してしまいます。",
        "Sea system melts all of your enemies and returns them to the great ocean.")},
    { ElementRealm::DARKNESS,
    _("闇系統は恐るべき力を常闇から引き出し、敵を地獄へと叩き落とすでしょう。",
        "Dark system draws terrifying power from the darkness and knocks your enemies into hell.")},
    { ElementRealm::CHAOS,
    _("混沌系統は敵の意識も条理も捻じ曲げ、その存在をあの世に送ってしまいます。",
        "Chaos system twists and wraps your enemies, even their souls, and scatters them as dust in the wind.")},
    { ElementRealm::EARTH,
    _("地系統は偉大なる大地の力を呼び出して、数多の敵のことごとくを粉砕しようとします。",
        "Earth system smashes all of your enemies massively using its huge powers.")},
    { ElementRealm::DEATH,
    _("瘴気系統は全ての生ける者にとって途轍もない毒です。",
        "Death system is a tremendous poison for all living enemies.")},
};

// clang-format on

/*!
 * @brief 元素魔法の領域名を返す
 * @param realm_idx 領域番号
 * @return 領域名
 */
concptr get_element_title(int realm_idx)
{
    auto realm = static_cast<ElementRealm>(realm_idx);
    return element_types.at(realm).title.data();
}

/*!
 * @brief 元素魔法領域の属性リストを返す
 * @param realm_idx 領域番号
 * @return 領域で使用できる属性リスト
 */
static std::array<spells_type, 3> get_element_types(int realm_idx)
{
    auto realm = static_cast<ElementRealm>(realm_idx);
    return element_types.at(realm).type;
}

/*!
 * @brief 元素魔法領域のn番目の属性を返す
 * @param realm_idx 領域番号
 * @param n 属性の何番目か
 * @return 属性タイプ
 */
spells_type get_element_type(int realm_idx, int n)
{
    return get_element_types(realm_idx)[n];
}

/*!
 * @brief 元素魔法領域のn番目の呪文用の属性を返す
 * @param realm_idx 領域番号
 * @param n 属性の何番目か
 * @return 属性タイプ
 */
static spells_type get_element_spells_type(player_type *caster_ptr, int n) {
    auto realm = element_types.at(static_cast<ElementRealm>(caster_ptr->element));
    auto t = realm.type.at(n);
    if (realm.extra.find(t) != realm.extra.end()) {
        if (randint0(100) < caster_ptr->lev * 2)
            return realm.extra.at(t);
    }
    return t;
}

/*!
 * @brief 元素魔法領域の属性名リストを返す
 * @param realm_idx 領域番号
 * @return 領域で使用できる属性の名称リスト
 */
static std::array<std::string_view, 3> get_element_names(int realm_idx)
{
    auto realm = static_cast<ElementRealm>(realm_idx);
    return element_types.at(realm).name;
}

/*!
 * @brief 元素魔法領域のn番目の属性名を返す
 * @param realm_idx 領域番号
 * @param n 属性の何番目か
 * @return 属性名
 */
concptr get_element_name(int realm_idx, int n)
{
    return get_element_names(realm_idx)[n].data();
}

/*!
 * @brief 元素魔法の説明文を取得
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 説明文
 */
static concptr get_element_tip(player_type *caster_ptr, int spell_idx)
{
    auto realm = static_cast<ElementRealm>(caster_ptr->element);
    auto spell = static_cast<ElementSpells>(spell_idx);
    auto elem = element_powers.at(spell).elem;
    return format(element_tips.at(spell).data(), element_types.at(realm).name[elem].data());
}

/*!
 * @brief 元素魔法の説明文を取得
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 説明文
 */
static int get_elemental_elem(player_type *caster_ptr, int spell_idx)
{
    (void)caster_ptr;
    auto spell = static_cast<ElementSpells>(spell_idx);
    return element_powers.at(spell).elem;
}

/*!
 * @brief 元素魔法呪文の難易度データを取得
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 説明文
 */
static mind_type get_elemental_info(player_type *caster_ptr, int spell_idx)
{
    (void)caster_ptr;
    auto spell = static_cast<ElementSpells>(spell_idx);
    return element_powers.at(spell).info;
}

/*!
 * @brief 元素魔法呪文の効果表示文字列を取得
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @param p バッファ
 * @return なし(pを更新)
 */
void get_element_effect_info(player_type *caster_ptr, int spell_idx, char *p)
{
    PLAYER_LEVEL plev = caster_ptr->lev;
    auto spell = static_cast<ElementSpells>(spell_idx);
    int dam = 0;

    switch (spell) {
    case ElementSpells::BOLT_1ST:
        sprintf(p, " %s%dd%d", KWD_DAM, 3 + ((plev - 1) / 5), 4);
        break;
    case ElementSpells::CURE:
        sprintf(p, " %s%dd%d", KWD_HEAL, 2, 8);
        break;
    case ElementSpells::BOLT_2ND:
        sprintf(p, " %s%dd%d", KWD_DAM, 8 + ((plev - 5) / 4), 8);
        break;
    case ElementSpells::BALL_3RD:
        sprintf(p, " %s%d", KWD_DAM, (50 + plev * 2));
        break;
    case ElementSpells::BALL_1ST:
        sprintf(p, " %s%d", KWD_DAM, 55 + plev);
        break;
    case ElementSpells::BREATH_2ND:
        dam = p_ptr->chp / 2;
        sprintf(p, " %s%d", KWD_DAM, (dam > 150) ? 150 : dam);
        break;
    case ElementSpells::ANNIHILATE:
        sprintf(p, " %s%d", _("効力:", "pow "), 50 + plev);
        break;
    case ElementSpells::BOLT_3RD:
        sprintf(p, " %s%dd%d", KWD_DAM, 12 + ((plev - 5) / 4), 8);
        break;
    case ElementSpells::WAVE_1ST:
        sprintf(p, " %s50+d%d", KWD_DAM, plev * 3);
        break;
    case ElementSpells::BALL_2ND:
        sprintf(p, " %s%d", KWD_DAM, 75 + plev * 3 / 2);
        break;
    case ElementSpells::BURST_1ST:
        sprintf(p, " %s%dd%d", KWD_DAM, 6 + plev / 8, 7);
        break;
    case ElementSpells::STORM_2ND:
        sprintf(p, " %s%d", KWD_DAM, 115 + plev * 5 / 2);
        break;
    case ElementSpells::BREATH_1ST:
        sprintf(p, " %s%d", KWD_DAM, p_ptr->chp * 2 / 3);
        break;
    case ElementSpells::STORM_3ND:
        sprintf(p, " %s%d", KWD_DAM, 300 + plev * 5);
        break;
    default:
        p[0] = '\0';
        break;
    }
}

/*!
 * @brief 元素魔法呪文を実行する
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 実行したらTRUE、キャンセルならFALSE
 */
static bool cast_element_spell(player_type *caster_ptr, SPELL_IDX spell_idx)
{
    auto spell = static_cast<ElementSpells>(spell_idx);
    auto power = element_powers.at(spell);
    spells_type typ;
    DIRECTION dir;
    PLAYER_LEVEL plev = caster_ptr->lev;
    HIT_POINT dam;
    POSITION y, x;
    int num;

    switch (spell) {
    case ElementSpells::BOLT_1ST:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = damroll(3 + ((plev - 1) / 5), 4);
        typ = get_element_spells_type(caster_ptr, power.elem);
        (void)fire_bolt(caster_ptr, typ, dir, dam);
        break;
    case ElementSpells::MON_DETECT:
        (void)detect_monsters_normal(caster_ptr, DETECT_RAD_DEFAULT);
        (void)detect_monsters_invis(caster_ptr, DETECT_RAD_DEFAULT);
        break;
    case ElementSpells::PERCEPT:
        return psychometry(caster_ptr);
    case ElementSpells::CURE:
        (void)hp_player(caster_ptr, damroll(2, 8));
        (void)set_cut(caster_ptr, caster_ptr->cut - 10);
        break;
    case ElementSpells::BOLT_2ND:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = damroll(8 + ((plev - 5) / 4), 8);
        typ = get_element_spells_type(caster_ptr, power.elem);
        if (fire_bolt_or_beam(caster_ptr, plev, typ, dir, dam)) {
            if (typ == GF_HYPODYNAMIA) {
                (void)hp_player(caster_ptr, dam / 2);
            }
        }
        break;
    case ElementSpells::MAG_DETECT:
        (void)detect_objects_magic(caster_ptr, DETECT_RAD_DEFAULT);
        break;
    case ElementSpells::BALL_3RD:
        project_length = 4;
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        typ = get_element_spells_type(caster_ptr, power.elem);
        dam = 50 + plev * 2;
        (void)fire_ball(caster_ptr, typ, dir, dam, 1);
        project_length = 0;
        break;
    case ElementSpells::BALL_1ST:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = 55 + plev;
        typ = get_element_spells_type(caster_ptr, power.elem);
        (void)fire_ball(caster_ptr, typ, dir, dam, 2);
        break;
    case ElementSpells::BREATH_2ND:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = MIN(150, caster_ptr->chp / 2);
        typ = get_element_spells_type(caster_ptr, power.elem);
        if (fire_breath(caster_ptr, typ, dir, dam, 3)) {
            if (typ == GF_HYPODYNAMIA) {
                (void)hp_player(caster_ptr, dam / 2);
            }
        }
        break;
    case ElementSpells::ANNIHILATE:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        fire_ball_hide(caster_ptr, GF_E_GENOCIDE, dir, plev + 50, 0);
        break;
    case ElementSpells::BOLT_3RD:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = damroll(12 + ((plev - 5) / 4), 8);
        typ = get_element_spells_type(caster_ptr, power.elem);
        fire_bolt_or_beam(caster_ptr, plev, typ, dir, dam);
        break;
    case ElementSpells::WAVE_1ST:
        dam = 50 + randint1(plev * 3);
        typ = get_element_spells_type(caster_ptr, power.elem);
        project_all_los(caster_ptr, typ, dam);
        break;
    case ElementSpells::BALL_2ND:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = 75 + plev * 3 / 2;
        typ = get_element_spells_type(caster_ptr, power.elem);
        if (fire_ball(caster_ptr, typ, dir, dam, 3)) {
            if (typ == GF_HYPODYNAMIA) {
                (void)hp_player(caster_ptr, dam / 2);
            }
        }
        break;
    case ElementSpells::BURST_1ST:
        y = caster_ptr->y;
        x = caster_ptr->x;
        num = damroll(4, 3);
        typ = get_element_spells_type(caster_ptr, power.elem);
        for (int k = 0; k < num; k++) {
            int attempts = 1000;
            while (attempts--) {
                scatter(caster_ptr, &y, &x, caster_ptr->y, caster_ptr->x, 4, PROJECT_NONE);
                if (!cave_has_flag_bold(caster_ptr->current_floor_ptr, y, x, FF_PROJECT))
                    continue;
                if (!player_bold(caster_ptr, y, x))
                    break;
            }
            project(caster_ptr, 0, 0, y, x, damroll(6 + plev / 8, 7), typ, (PROJECT_BEAM | PROJECT_THRU | PROJECT_GRID | PROJECT_KILL), -1);
        }
        break;
    case ElementSpells::STORM_2ND:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = 115 + plev * 5 / 2;
        typ = get_element_spells_type(caster_ptr, power.elem);
        if (fire_ball(caster_ptr, typ, dir, dam, 4)) {
            if (typ == GF_HYPODYNAMIA) {
                (void)hp_player(caster_ptr, dam / 2);
            }
        }
        break;
    case ElementSpells::BREATH_1ST:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = caster_ptr->chp * 2 / 3;
        typ = get_element_spells_type(caster_ptr, power.elem);
        (void)fire_breath(caster_ptr, typ, dir, dam, 3);
        break;
    case ElementSpells::STORM_3ND:
        if (!get_aim_dir(caster_ptr, &dir))
            return FALSE;
        dam = 300 + plev * 5;
        typ = get_element_spells_type(caster_ptr, power.elem);
        (void)fire_ball(caster_ptr, typ, dir, dam, 5);
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

/*!
 * @brief 元素魔法呪文の失敗率を計算
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 失敗率
 */
static PERCENTAGE decide_element_chance(player_type *caster_ptr, mind_type spell)
{
    PERCENTAGE chance = spell.fail;

    chance -= 3 * (caster_ptr->lev - spell.min_lev);
    chance += caster_ptr->to_m_chance;
    chance -= 3 * (adj_mag_stat[caster_ptr->stat_index[A_WIS]] - 1);
    
    PERCENTAGE minfail = adj_mag_fail[caster_ptr->stat_index[A_WIS]];
    if (chance < minfail)
        chance = minfail;

    if (caster_ptr->stun > 50)
        chance += 25;
    else if (caster_ptr->stun)
        chance += 15;

    if (heavy_armor(caster_ptr))
        chance += 5;

    if (caster_ptr->icky_wield[0])
        chance += 5;

    if (caster_ptr->icky_wield[1])
        chance += 5;

    if (chance > 95)
        chance = 95;

    return chance;
}

/*!
 * @brief 元素魔法呪文の消費MPを計算
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @return 消費MP
 */
static MANA_POINT decide_element_mana_cost(player_type *caster_ptr, mind_type spell)
{
    (void)caster_ptr;
    return spell.mana_cost;
}

/*!
 * @brief 元素魔法呪文を選択して取得
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param sn 呪文番号
 * @param only_browse 閲覧モードかどうか
 * @return 選んだらTRUE、選ばなかったらFALSE
 */
bool get_element_power(player_type *caster_ptr, SPELL_IDX *sn, bool only_browse)
{
    SPELL_IDX i;
    int num = 0;
    TERM_LEN y = 1;
    TERM_LEN x = 10;
    PLAYER_LEVEL plev = caster_ptr->lev;
    int ask = TRUE;
    char choice;
    char out_val[160];
    char comment[80];
    COMMAND_CODE code;
    bool flag, redraw;
    int menu_line = (use_menu ? 1 : 0);

    *sn = -1;
    if (repeat_pull(&code)) {
        *sn = (SPELL_IDX)code;
        if (get_elemental_info(caster_ptr, *sn).min_lev <= plev)
            return TRUE;
    }

    concptr p = _("元素魔法", "magic");
    flag = FALSE;
    redraw = FALSE;

    for (i = 0; i < static_cast<SPELL_IDX>(ElementSpells::MAX); i++) {
        if (get_elemental_info(caster_ptr, i).min_lev <= plev)
            num++;
    }

    if (only_browse)
        (void)strnfmt(out_val, 78, 
            _("(%^s %c-%c, '*'で一覧, ESC) どの%sについて知りますか？", "(%^ss %c-%c, *=List, ESC=exit) Use which %s? "),
            p, I2A(0), I2A(num - 1), p);
    else
        (void)strnfmt(out_val, 78, 
            _("(%^s %c-%c, '*'で一覧, ESC) どの%sを使いますか？", "(%^ss %c-%c, *=List, ESC=exit) Use which %s? "),
            p, I2A(0), I2A(num - 1), p);

    if (use_menu && !only_browse)
        screen_save();

    int elem;
    mind_type spell;
    choice = (always_show_list || use_menu) ? ESCAPE : 1;
    while (!flag) {
        if (choice == ESCAPE)
            choice = ' ';
        else if (!get_com(out_val, &choice, TRUE))
            break;

        if (use_menu && choice != ' ') {
            switch (choice) {
            case '0':
                if (!only_browse)
                    screen_load();
                return FALSE;
            case '8':
            case 'k':
            case 'K':
                menu_line += (num - 1);
                break;
            case '2':
            case 'j':
            case 'J':
                menu_line++;
                break;
            case 'x':
            case 'X':
            case '\r':
            case '\n':
                i = menu_line - 1;
                ask = FALSE;
                break;
            }

            if (menu_line > num)
                menu_line -= num;
        }

        int spell_max = static_cast<int>(ElementSpells::MAX);
        if ((choice == ' ') || (choice == '*') || (choice == '?') || (use_menu && ask)) {
            if (!redraw || use_menu) {
                char desc[80];
                char name[80];
                redraw = TRUE;
                if (!only_browse && !use_menu)
                    screen_save();

                prt("", y, x);
                put_str(_("名前", "Name"), y, x + 5);
                put_str(_("Lv   MP   失率 効果", "Lv   MP   Fail Info"), y, x + 35);
                for (i = 0; i < spell_max; i++) {
                    elem = get_elemental_elem(caster_ptr, i);
                    spell = get_elemental_info(caster_ptr, i);

                    if (spell.min_lev > plev)
                        break;

                    PERCENTAGE chance = decide_element_chance(caster_ptr, spell);
                    int mana_cost = decide_element_mana_cost(caster_ptr, spell);
                    get_element_effect_info(caster_ptr, i, comment);

                    if (use_menu) {
                        if (i == (menu_line - 1))
                            strcpy(desc, _("  》 ", "  >  "));
                        else
                            strcpy(desc, "     ");
                    } else
                        sprintf(desc, "  %c) ", I2A(i));

                    concptr s = get_element_name(caster_ptr->element, elem);
                    sprintf(name, spell.name, s);
                    strcat(desc,
                        format("%-30s%2d %4d %3d%%%s", name, spell.min_lev, mana_cost, chance, comment));
                    prt(desc, y + i + 1, x);
                }

                prt("", y + i + 1, x);
            } else if (!only_browse) {
                redraw = FALSE;
                screen_load();
            }

            continue;
        }

        if (!use_menu) {
            ask = isupper(choice);
            if (ask)
                choice = (char)tolower(choice);

            i = (islower(choice) ? A2I(choice) : -1);
        }

        if ((i < 0) || (i >= num)) {
            bell();
            continue;
        }

        if (ask) {
            char name[80];
            char tmp_val[160];
            elem = get_elemental_elem(caster_ptr, i);
            spell = get_elemental_info(caster_ptr, i);
            (void)sprintf(name, spell.name, get_element_name(caster_ptr->element, elem));
            (void)strnfmt(tmp_val, 78, _("%sを使いますか？", "Use %s? "), name);
            if (!get_check(tmp_val))
                continue;
        }

        flag = TRUE;
    }

    if (redraw && !only_browse)
        screen_load();

    set_bits(caster_ptr->window_flags, PW_SPELL);
    handle_stuff(caster_ptr);
    if (!flag)
        return FALSE;

    *sn = i;
    repeat_push((COMMAND_CODE)i);
    return TRUE;
}

/*!
 * @brief 元素魔法呪文をMPがなくても挑戦するか確認する
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param mana_cost 消費MP
 * @return 詠唱するならTRUE、しないならFALSE
 */
static bool check_element_mp_sufficiency(player_type *caster_ptr, int mana_cost)
{
    if (mana_cost <= caster_ptr->csp)
        return TRUE;

    msg_print(_("ＭＰが足りません。", "You do not have enough mana to use this power."));
    if (!over_exert)
        return FALSE;

    return get_check(_("それでも挑戦しますか? ", "Attempt it anyway? "));
}

/*!
 * @brief 元素魔法呪文の詠唱を試み、成功なら詠唱し、失敗ならファンブルする
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param spell_idx 呪文番号
 * @param chance 失敗率
 * @return 詠唱して実行したらTRUE、されなかったらFALSE
 */
static bool try_cast_element_spell(player_type *caster_ptr, SPELL_IDX spell_idx, PERCENTAGE chance)
{
    if (randint0(100) >= chance) {
        sound(SOUND_ZAP);
        return cast_element_spell(caster_ptr, spell_idx);
    }

    if (flush_failure)
        flush();

    msg_format(_("魔力の集中に失敗した！", "You failed to concentrate hard enough for Mana!"));
    sound(SOUND_FAIL);

    if (randint1(100) < chance / 2) {
        int plev = caster_ptr->lev;
        msg_print(_("元素の力が制御できない氾流となって解放された！",
            "Elemental power unleashes its power in an uncontrollable storm!"));
        project(caster_ptr, PROJECT_WHO_UNCTRL_POWER, 2 + plev / 10, caster_ptr->y, caster_ptr->x, plev * 2,
            get_element_types(caster_ptr->element)[0],
            PROJECT_JUMP | PROJECT_KILL | PROJECT_GRID | PROJECT_ITEM, -1);
        caster_ptr->csp = MAX(0, caster_ptr->csp - caster_ptr->msp * 10 / (20 + randint1(10)));

        take_turn(caster_ptr, 100);
        set_bits(caster_ptr->redraw, PR_MANA);
        set_bits(caster_ptr->window_flags, PW_PLAYER | PW_SPELL);

        return FALSE;
    }

    return TRUE;
}

/*!
 * @brief 元素魔法コマンドのメインルーチン
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @return なし
 */
void do_cmd_element(player_type *caster_ptr)
{
    SPELL_IDX i;
    if (cmd_limit_confused(caster_ptr) || !get_element_power(caster_ptr, &i, FALSE))
        return;

    mind_type spell = get_elemental_info(caster_ptr, i);
    PERCENTAGE chance = decide_element_chance(caster_ptr, spell);
    int mana_cost = decide_element_mana_cost(caster_ptr, spell);

    if (!check_element_mp_sufficiency(caster_ptr, mana_cost))
        return;

    if (!try_cast_element_spell(caster_ptr, i, chance))
        return;

    if (mana_cost <= caster_ptr->csp) {
        caster_ptr->csp -= mana_cost;
    } else {
        int oops = mana_cost;
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

    take_turn(caster_ptr, 100);
    set_bits(caster_ptr->redraw, PR_MANA);
    set_bits(caster_ptr->window_flags, PW_PLAYER | PW_SPELL);
}

/*!
 * @brief 現在プレイヤーが使用可能な元素魔法の一覧表示
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @return なし
 */
void do_cmd_element_browse(player_type *caster_ptr)
{
    SPELL_IDX n = 0;
    char temp[62 * 5];

    screen_save();
    while (TRUE) {
        if (!get_element_power(caster_ptr, &n, TRUE)) {
            screen_load();
            return;
        }

        term_erase(12, 21, 255);
        term_erase(12, 20, 255);
        term_erase(12, 19, 255);
        term_erase(12, 18, 255);
        term_erase(12, 17, 255);
        term_erase(12, 16, 255);
        shape_buffer(get_element_tip(caster_ptr, n), 62, temp, sizeof(temp));
        for (int j = 0, line = 17; temp[j]; j += (1 + strlen(&temp[j]))) {
            prt(&temp[j], line, 15);
            line++;
        }

        prt(_("何かキーを押して下さい。", "Hit any key."), 0, 0);
        (void)inkey();
    }
}

/*!
 * @brief 元素魔法の単体抹殺が有効か確認する
 * @param r_ptr モンスター種族への参照ポインタ
 * @param type 魔法攻撃属性
 * @return 効果があるならTRUE、なければFALSE
 */
bool is_elemental_genocide_effective(monster_race *r_ptr, spells_type type)
{
    switch (type) {
    case GF_FIRE:
        if (any_bits(r_ptr->flagsr, RFR_IM_FIRE))
            return FALSE;
        break;
    case GF_COLD:
        if (any_bits(r_ptr->flagsr, RFR_IM_COLD))
            return FALSE;
        break;
    case GF_ELEC:
        if (any_bits(r_ptr->flagsr, RFR_IM_ELEC))
            return FALSE;
        break;
    case GF_ACID:
        if (any_bits(r_ptr->flagsr, RFR_IM_ACID))
            return FALSE;
        break;
    case GF_DARK:
        if (any_bits(r_ptr->flagsr, RFR_RES_DARK) || any_bits(r_ptr->r_flags3, RF3_HURT_LITE))
            return FALSE;
        break;
    case GF_CONFUSION:
        if (any_bits(r_ptr->flags3, RF3_NO_CONF))
            return FALSE;
        break;
    case GF_SHARDS:
        if (any_bits(r_ptr->flagsr, RFR_RES_SHAR))
            return FALSE;
        break;
    case GF_POIS:
        if (any_bits(r_ptr->flagsr, RFR_IM_POIS))
            return FALSE;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

/*!
 * @brief 元素魔法の単体抹殺の効果を発動する 
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param em_ptr 魔法効果情報への参照ポインタ
 * @return 効果処理を続けるかどうか
 */
process_result effect_monster_elemental_genocide(player_type *caster_ptr, effect_monster_type *em_ptr)
{
    auto type = get_element_type(caster_ptr->element, 0);
    auto name = get_element_name(caster_ptr->element, 0);
    bool b = is_elemental_genocide_effective(em_ptr->r_ptr, type);

    if (em_ptr->seen_msg)
        msg_format(_("%sが%sを包み込んだ。", "The %s surrounds %s."), name, em_ptr->m_name);

    if (em_ptr->seen)
        em_ptr->obvious = TRUE;

    if (!b) {
        if (em_ptr->seen_msg)
            msg_format(_("%sには効果がなかった。", "%^s is unaffected."), em_ptr->m_name);
        em_ptr->dam = 0;
        return PROCESS_TRUE;
    }

    if (genocide_aux(caster_ptr, em_ptr->g_ptr->m_idx, em_ptr->dam, !em_ptr->who, (em_ptr->r_ptr->level + 1) / 2, _("モンスター消滅", "Genocide One"))) {
        if (em_ptr->seen_msg)
            msg_format(_("%sは消滅した！", "%^s disappeared!"), em_ptr->m_name);
        em_ptr->dam = 0;
        chg_virtue(caster_ptr, V_VITALITY, -1);
        return PROCESS_TRUE;
    }

    em_ptr->skipped = TRUE;
    return PROCESS_CONTINUE;
}

/*!
 * @brief 元素領域とレベルの条件に見合うかチェックする
 * @param caster_ptr プレイヤー情報への参照ポインタ
 * @param realm 領域
 * @param lev プレイヤーレベル
 * @return 見合うならTRUE、そうでなければFALSE
 * @detail
 * レベルに応じて取得する耐性などの判定に使用する
 */
bool has_element_resist(player_type *creature_ptr, ElementRealm realm, PLAYER_LEVEL lev)
{
    if (creature_ptr->pclass != CLASS_ELEMENTALIST)
        return FALSE;

    auto prealm = static_cast<ElementRealm>(creature_ptr->element);
    return (prealm == realm && creature_ptr->lev >= lev);
}

/*!
 * @brief 領域選択時のカーソル表示(シンボル＋領域名)
 * @param i 位置
 * @param n 最後尾の位置
 * @param color 表示色
 * @return なし
 */
static void display_realm_cursor(int i, int n, term_color_type color)
{
    char cur[80];
    char sym;
    concptr name;
    if (i == n) {
        sym = '*';
        name = _("ランダム", "Random");
    } else {
        sym = I2A(i);
        name = element_types.at(static_cast<ElementRealm>(i + 1)).title.data();
    }
    sprintf(cur, "%c) %s", sym, name);

    c_put_str(color, cur, 12 + (i / 5), 2 + 15 * (i % 5));
}

/*!
 * @brief 領域選択時の移動キー処理
 * @param cs 現在位置
 * @param n 最後尾の位置
 * @param c 入力キー
 * @return 新しい位置
 */
static int interpret_realm_select_key(int cs, int n, char c)
{
    if (c == 'Q')
        quit(NULL);

    if (c == '8')
        if (cs >= 5)
            return cs - 5;

    if (c == '4')
        if (cs > 0)
            return cs - 1;

    if (c == '6')
        if (cs < n)
            return cs + 1;

    if (c == '2')
        if (cs + 5 <= n)
            return cs + 5;

    return cs;
}

/*!
 * @brief 領域選択ループ処理
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param n 最後尾の位置
 * @return 領域番号
 */
static int get_element_realm(player_type *creature_ptr, int is, int n)
{
    int cs = MAX(0, is);
    int os = cs;
    int k;

    char buf[80];
    sprintf(buf, _("領域を選んで下さい(%c-%c) ('='初期オプション設定): ", "Choose a realm (%c-%c) ('=' for options): "), I2A(0), I2A(n - 1));

    while (TRUE) {
        display_realm_cursor(os, n, TERM_WHITE);
        display_realm_cursor(cs, n, TERM_YELLOW);
        put_str(buf, 10, 10);
        os = cs;

        char c = inkey();
        cs = interpret_realm_select_key(cs, n, c);

        if (c == 'S')
            return 255;

        if (c == ' ' || c == '\r' || c == '\n') {
            if (cs == n) {
                display_realm_cursor(cs, n, TERM_WHITE);
                cs = randint0(n - 1);
            }
            break;
        }

        if (c == '*') {
            display_realm_cursor(cs, n, TERM_WHITE);
            cs = randint0(n - 1);
            break;
        }

        k = islower(c) ? A2I(c) : -1;
        if (k >= 0 && k < n) {
            display_realm_cursor(cs, n, TERM_WHITE);
            cs = k;
            break;
        }

        k = isupper(c) ? (26 + c - 'A') : -1;
        if (k >= 26 && k < n) {
            display_realm_cursor(cs, n, TERM_WHITE);
            cs = k;
            break;
        }

        if (c == '=') {
            screen_save();
            do_cmd_options_aux(creature_ptr, OPT_PAGE_BIRTH, _("初期オプション((*)はスコアに影響)", "Birth Options ((*)) affect score"));
            screen_load();
        } else if (c != '2' && c != '4' && c != '6' && c != '8')
            bell();
    }

    display_realm_cursor(cs, n, TERM_YELLOW);
    return (cs + 1);
}

/*!
 * @brief 領域選択
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @return 領域番号
 */
byte select_element_realm(player_type *creature_ptr)
{
    clear_from(10);

    int realm_max = static_cast<int>(ElementRealm::MAX);
    int realm_idx = 1;
    int row = 16;
    while (1) {
        put_str(
            _("注意：元素系統の選択によりあなたが習得する呪文のタイプが決まります。", "Note: The system of element will determine which spells you can learn."),
            23, 5);

        for (int i = 0; i < realm_max; i++) {
            display_realm_cursor(i, realm_max - 1, TERM_WHITE);
        }

        realm_idx = get_element_realm(creature_ptr, realm_idx - 1, realm_max - 1);
        if (realm_idx == 255)
            break;

        auto realm = static_cast<ElementRealm>(realm_idx);
        char temp[80 * 5];
        shape_buffer(element_texts.at(realm).data(), 74, temp, sizeof(temp));
        concptr t = temp;
        for (int i = 0; i < 5; i++) {
            if (t[0] == 0)
                break;
            prt(t, row + i, 3);
            t += strlen(t) + 1;
        }

        if (get_check_strict(creature_ptr, _("よろしいですか？", "Are you sure? "), CHECK_DEFAULT_Y))
            break;

        clear_from(row);
    }

    clear_from(10);
    return (byte)realm_idx;
}

/*!
 * @brief クラスパワー情報を追加
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param rc_ptr レイシャルパワー情報への参照ポインタ
 * @return なし
 */
void switch_element_racial(player_type *creature_ptr, rc_type *rc_ptr)
{
    auto plev = creature_ptr->lev;
    auto realm = static_cast<ElementRealm>(creature_ptr->element);
    switch (realm) {
    case ElementRealm::FIRE:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("ライト・エリア", "Light area"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 3;
        rc_ptr->power_desc[rc_ptr->num].cost = 5;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 10;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::ICE:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("周辺フリーズ", "Sleep monsters"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 10;
        rc_ptr->power_desc[rc_ptr->num].cost = 15;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 25;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::SKY:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("魔力充填", "Recharging"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 20;
        rc_ptr->power_desc[rc_ptr->num].cost = 15;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 25;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::SEA:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("岩石溶解", "Stone to mud"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 5;
        rc_ptr->power_desc[rc_ptr->num].cost = 5;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 10;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::DARKNESS:
        sprintf(rc_ptr->power_desc[rc_ptr->num].racial_name, _("闇の扉(半径%d)", "Door to darkness(rad %d)"), 15 + plev / 2);
        rc_ptr->power_desc[rc_ptr->num].min_level = 5;
        rc_ptr->power_desc[rc_ptr->num].cost = 5 + plev / 7;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 20;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::CHAOS:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("現実変容", "Alter reality"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 35;
        rc_ptr->power_desc[rc_ptr->num].cost = 30;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 40;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::EARTH:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("地震", "Earthquake"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 25;
        rc_ptr->power_desc[rc_ptr->num].cost = 15;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 20;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    case ElementRealm::DEATH:
        strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("増殖阻止", "Sterilization"));
        rc_ptr->power_desc[rc_ptr->num].min_level = 5;
        rc_ptr->power_desc[rc_ptr->num].cost = 5;
        rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
        rc_ptr->power_desc[rc_ptr->num].fail = 20;
        rc_ptr->power_desc[rc_ptr->num++].number = -4;
        break;
    default:
        break;
    }
}

/*!
 * @todo 宣言だけ。後日適切な場所に移動
 */
static bool door_to_darkness(player_type *caster_ptr, POSITION dist);

/*!
 * @brief クラスパワーを実行
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @return 実行したらTRUE、しなかったらFALSE
 */
bool switch_element_execution(player_type *creature_ptr)
{
    auto realm = static_cast<ElementRealm>(creature_ptr->element);
    PLAYER_LEVEL plev = creature_ptr->lev;
    DIRECTION dir;

    switch (realm) {
    case ElementRealm::FIRE:
        (void)lite_area(creature_ptr, damroll(2, plev / 2), plev / 10);
        break;
    case ElementRealm::ICE:
        (void)project(creature_ptr, 0, 5, creature_ptr->y, creature_ptr->x, 1, GF_COLD, PROJECT_ITEM, -1);
        (void)project_all_los(creature_ptr, GF_OLD_SLEEP, 20 + plev * 3 / 2);
        break;
    case ElementRealm::SKY:
        (void)recharge(creature_ptr, 120);
        break;
    case ElementRealm::SEA:
        if (!get_aim_dir(creature_ptr, &dir))
            return FALSE;
        (void)wall_to_mud(creature_ptr, dir, plev * 3 / 2);
        break;
    case ElementRealm::DARKNESS:
        return door_to_darkness(creature_ptr, 15 + plev / 2);
        break;
    case ElementRealm::CHAOS:
        reserve_alter_reality(creature_ptr, randint0(21) + 15);
        break;
    case ElementRealm::EARTH:
        (void)earthquake(creature_ptr, creature_ptr->y, creature_ptr->x, 10, 0);
        break;
    case ElementRealm::DEATH:
        if (creature_ptr->current_floor_ptr->num_repro <= MAX_REPRO)
            creature_ptr->current_floor_ptr->num_repro += MAX_REPRO;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

/*!
 * @brief 指定したマスが暗いかどうか
 * @param f_ptr 階の情報への参照ポインタ
 * @param y 指定のy座標
 * @param x 指定のx座標
 * @return 暗いならTRUE、そうでないならFALSE
 */
static bool is_target_grid_dark(floor_type* f_ptr, POSITION y, POSITION x)
{
    if (any_bits(f_ptr->grid_array[y][x].info, CAVE_MNLT))
        return FALSE;

    bool is_dark = FALSE;
    bool is_lite = any_bits(f_ptr->grid_array[y][x].info, CAVE_GLOW | CAVE_LITE);

    for (int dx = x - 2; dx <= x + 2; dx++)
        for (int dy = y - 2; dy <= y + 2; dy++) {
            if (dx == x && dy == y)
                continue;
            if (!in_bounds(f_ptr, dy, dx))
                continue;

            MONSTER_IDX m_idx = f_ptr->grid_array[dy][dx].m_idx;
            if (!m_idx)
                continue;

            POSITION d = distance(dy, dx, y, x);
            monster_race *r_ptr = &r_info[f_ptr->m_list[m_idx].r_idx];
            if (d <= 1 && any_bits(r_ptr->flags7, RF7_HAS_LITE_1 | RF7_SELF_LITE_1))
                return FALSE;
            if (d <= 2 && any_bits(r_ptr->flags7, RF7_HAS_LITE_2 | RF7_SELF_LITE_2))
                return FALSE;
            if (d <= 1 && any_bits(r_ptr->flags7, RF7_HAS_DARK_1 | RF7_SELF_DARK_1))
                is_dark = TRUE;
            if (d <= 2 && any_bits(r_ptr->flags7, RF7_HAS_DARK_2 | RF7_SELF_DARK_2))
                is_dark = TRUE;
        }

    return !is_lite || is_dark;
}

/*!
 * @breif 暗いところ限定での次元の扉
 * @param caster_ptr プレイヤー情報への参照ポインタ
 */
static bool door_to_darkness(player_type* caster_ptr, POSITION dist)
{
    POSITION y = caster_ptr->y;
    POSITION x = caster_ptr->x;
    floor_type *f_ptr;

    for (int i = 0; i < 3; i++) {
        if (!tgt_pt(caster_ptr, &x, &y))
            return FALSE;

        f_ptr = caster_ptr->current_floor_ptr;

        if (distance(y, x, caster_ptr->y, caster_ptr->x) > dist) {
            msg_print(_("遠すぎる！", "There is too far!"));
            continue;
        }

        if (!is_cave_empty_bold(caster_ptr, y, x) || f_ptr->grid_array[y][x].info & CAVE_ICKY) {
            msg_print(_("そこには移動できない。", "Can not teleport to there."));
            continue;
        }

        break;
    }

    bool flag = cave_player_teleportable_bold(caster_ptr, y, x, TELEPORT_SPONTANEOUS)  && is_target_grid_dark(f_ptr, y, x);
    if (flag) {
        teleport_player_to(caster_ptr, y, x, TELEPORT_SPONTANEOUS);
    } else {
        msg_print(_("闇の扉は開かなかった！", "Door to darkness does not open!"));
    }
    return TRUE;
}
