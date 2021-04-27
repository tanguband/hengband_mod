﻿/*!
 * @brief モンスターのスペル振り分け処理 / Spell launch by a monster
 * @date 2014/07/14
 * @author Habu
 */

#include "mspell/assign-monster-spell.h"
#include "blue-magic/blue-magic-checker.h"
#include "monster-race/race-ability-flags.h"
#include "mspell/mspell-ball.h"
#include "mspell/mspell-bolt.h"
#include "mspell/mspell-breath.h"
#include "mspell/mspell-curse.h"
#include "mspell/mspell-dispel.h"
#include "mspell/mspell-floor.h"
#include "mspell/mspell-learn-checker.h"
#include "mspell/mspell-particularity.h"
#include "mspell/mspell-special.h"
#include "mspell/mspell-status.h"
#include "mspell/mspell-summon.h"
#include "mspell/mspell-util.h"
#include "mspell/mspell.h"
#include "spell/spell-types.h"
#include "system/player-type-definition.h"

/*!
 * @brief SPELL_IDX を monster_spell_type に変換する。
 */
static int spell_idx_to_monster_spell_type(const RF_ABILITY idx)
{
    return static_cast<int>(idx);
}

static MonsterSpellResult monspell_to_player_impl(player_type *target_ptr, RF_ABILITY ms_type, POSITION y, POSITION x, MONSTER_IDX m_idx)
{
    // clang-format off
    switch (ms_type) {
    case RF_ABILITY::SHRIEK: return spell_RF4_SHRIEK(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF4_SHRIEK */
    case RF_ABILITY::XXX1: break;   /* RF4_XXX1 */
    case RF_ABILITY::DISPEL: return spell_RF4_DISPEL(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF4_DISPEL */
    case RF_ABILITY::ROCKET: return spell_RF4_ROCKET(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_ROCKET */
    case RF_ABILITY::SHOOT: return spell_RF4_SHOOT(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_SHOOT */
    case RF_ABILITY::XXX2: break;   /* RF4_XXX2 */
    case RF_ABILITY::XXX3: break;   /* RF4_XXX3 */
    case RF_ABILITY::XXX4: break;   /* RF4_XXX4 */
    case RF_ABILITY::BR_ACID: return spell_RF4_BREATH(target_ptr, GF_ACID, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ACID */
    case RF_ABILITY::BR_ELEC: return spell_RF4_BREATH(target_ptr, GF_ELEC, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ELEC */
    case RF_ABILITY::BR_FIRE: return spell_RF4_BREATH(target_ptr, GF_FIRE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_FIRE */
    case RF_ABILITY::BR_COLD: return spell_RF4_BREATH(target_ptr, GF_COLD, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_COLD */
    case RF_ABILITY::BR_POIS: return spell_RF4_BREATH(target_ptr, GF_POIS, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_POIS */
    case RF_ABILITY::BR_NETH: return spell_RF4_BREATH(target_ptr, GF_NETHER, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_NETH */
    case RF_ABILITY::BR_LITE: return spell_RF4_BREATH(target_ptr, GF_LITE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_LITE */
    case RF_ABILITY::BR_DARK: return spell_RF4_BREATH(target_ptr, GF_DARK, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DARK */
    case RF_ABILITY::BR_CONF: return spell_RF4_BREATH(target_ptr, GF_CONFUSION, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_CONF */
    case RF_ABILITY::BR_SOUN: return spell_RF4_BREATH(target_ptr, GF_SOUND, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_SOUN */
    case RF_ABILITY::BR_CHAO: return spell_RF4_BREATH(target_ptr, GF_CHAOS, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_CHAO */
    case RF_ABILITY::BR_DISE: return spell_RF4_BREATH(target_ptr, GF_DISENCHANT, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_DISE */
    case RF_ABILITY::BR_NEXU: return spell_RF4_BREATH(target_ptr, GF_NEXUS, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_NEXU */
    case RF_ABILITY::BR_TIME: return spell_RF4_BREATH(target_ptr, GF_TIME, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_TIME */
    case RF_ABILITY::BR_INER: return spell_RF4_BREATH(target_ptr, GF_INERTIAL, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_INER */
    case RF_ABILITY::BR_GRAV: return spell_RF4_BREATH(target_ptr, GF_GRAVITY, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_GRAV */
    case RF_ABILITY::BR_SHAR: return spell_RF4_BREATH(target_ptr, GF_SHARDS, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_SHAR */
    case RF_ABILITY::BR_PLAS: return spell_RF4_BREATH(target_ptr, GF_PLASMA, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_PLAS */
    case RF_ABILITY::BR_FORC: return spell_RF4_BREATH(target_ptr, GF_FORCE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_WALL */
    case RF_ABILITY::BR_MANA: return spell_RF4_BREATH(target_ptr, GF_MANA, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_MANA */
    case RF_ABILITY::BA_NUKE: return spell_RF4_BA_NUKE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_NUKE */
    case RF_ABILITY::BR_NUKE: return spell_RF4_BREATH(target_ptr, GF_NUKE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_NUKE */
    case RF_ABILITY::BA_CHAO: return spell_RF4_BA_CHAO(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_CHAO */
    case RF_ABILITY::BR_DISI: return spell_RF4_BREATH(target_ptr, GF_DISINTEGRATE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DISI */
    case RF_ABILITY::BA_ACID: return spell_RF5_BA_ACID(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ACID */
    case RF_ABILITY::BA_ELEC: return spell_RF5_BA_ELEC(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ELEC */
    case RF_ABILITY::BA_FIRE: return spell_RF5_BA_FIRE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_FIRE */
    case RF_ABILITY::BA_COLD: return spell_RF5_BA_COLD(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_COLD */
    case RF_ABILITY::BA_POIS: return spell_RF5_BA_POIS(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_POIS */
    case RF_ABILITY::BA_NETH: return spell_RF5_BA_NETH(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_NETH */
    case RF_ABILITY::BA_WATE: return spell_RF5_BA_WATE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_WATE */
    case RF_ABILITY::BA_MANA: return spell_RF5_BA_MANA(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_MANA */
    case RF_ABILITY::BA_DARK: return spell_RF5_BA_DARK(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_DARK */
    case RF_ABILITY::DRAIN_MANA: return spell_RF5_DRAIN_MANA(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_DRAIN_MANA */
    case RF_ABILITY::MIND_BLAST: return spell_RF5_MIND_BLAST(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_MIND_BLAST */
    case RF_ABILITY::BRAIN_SMASH: return spell_RF5_BRAIN_SMASH(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MIND_BLAST */
    case RF_ABILITY::CAUSE_1: return spell_RF5_CAUSE_1(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_1 */
    case RF_ABILITY::CAUSE_2: return spell_RF5_CAUSE_2(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_2 */
    case RF_ABILITY::CAUSE_3: return spell_RF5_CAUSE_3(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_3 */
    case RF_ABILITY::CAUSE_4: return spell_RF5_CAUSE_4(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_4 */
    case RF_ABILITY::BO_ACID: return spell_RF5_BO_ACID(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ACID */
    case RF_ABILITY::BO_ELEC: return spell_RF5_BO_ELEC(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ELEC */
    case RF_ABILITY::BO_FIRE: return spell_RF5_BO_FIRE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_FIRE */
    case RF_ABILITY::BO_COLD: return spell_RF5_BO_COLD(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_COLD */
    case RF_ABILITY::BA_LITE: return spell_RF5_BA_LITE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_LITE */
    case RF_ABILITY::BO_NETH: return spell_RF5_BO_NETH(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_NETH */
    case RF_ABILITY::BO_WATE: return spell_RF5_BO_WATE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_WATE */
    case RF_ABILITY::BO_MANA: return spell_RF5_BO_MANA(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_MANA */
    case RF_ABILITY::BO_PLAS: return spell_RF5_BO_PLAS(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_PLAS */
    case RF_ABILITY::BO_ICEE: return spell_RF5_BO_ICEE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ICEE */
    case RF_ABILITY::MISSILE: return spell_RF5_MISSILE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MISSILE */
    case RF_ABILITY::SCARE: return spell_RF5_SCARE(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF5_SCARE */
    case RF_ABILITY::BLIND: return spell_RF5_BLIND(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF5_BLIND */
    case RF_ABILITY::CONF: return spell_RF5_CONF(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF5_CONF */
    case RF_ABILITY::SLOW: return spell_RF5_SLOW(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF5_SLOW */
    case RF_ABILITY::HOLD: return spell_RF5_HOLD(m_idx, target_ptr, 0, MONSTER_TO_PLAYER); /* RF5_HOLD */
    case RF_ABILITY::HASTE: return spell_RF6_HASTE(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_HASTE */
    case RF_ABILITY::HAND_DOOM: return spell_RF6_HAND_DOOM(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_HAND_DOOM */
    case RF_ABILITY::HEAL: return spell_RF6_HEAL(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_HEAL */
    case RF_ABILITY::INVULNER: return spell_RF6_INVULNER(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_INVULNER */
    case RF_ABILITY::BLINK: return spell_RF6_BLINK(target_ptr, m_idx, MONSTER_TO_PLAYER, FALSE); /* RF6_BLINK */
    case RF_ABILITY::TPORT: return spell_RF6_TPORT(target_ptr, m_idx, MONSTER_TO_PLAYER); /* RF6_TPORT */
    case RF_ABILITY::WORLD: return spell_RF6_WORLD(target_ptr, m_idx); /* RF6_WORLD */
    case RF_ABILITY::SPECIAL: return spell_RF6_SPECIAL(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF6_SPECIAL */
    case RF_ABILITY::TELE_TO: return spell_RF6_TELE_TO(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_TELE_TO */
    case RF_ABILITY::TELE_AWAY: return spell_RF6_TELE_AWAY(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_TELE_AWAY */
    case RF_ABILITY::TELE_LEVEL: return spell_RF6_TELE_LEVEL(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_TELE_LEVEL */
    case RF_ABILITY::PSY_SPEAR: return spell_RF6_PSY_SPEAR(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_PSY_SPEAR */
    case RF_ABILITY::DARKNESS: return spell_RF6_DARKNESS(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_DARKNESS */
    case RF_ABILITY::TRAPS: return spell_RF6_TRAPS(target_ptr, y, x, m_idx); /* RF6_TRAPS */
    case RF_ABILITY::FORGET: return spell_RF6_FORGET(target_ptr, m_idx); /* RF6_FORGET */
    case RF_ABILITY::RAISE_DEAD: return spell_RF6_RAISE_DEAD(target_ptr, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_RAISE_DEAD */
    case RF_ABILITY::S_KIN: return spell_RF6_S_KIN(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_KIN */
    case RF_ABILITY::S_CYBER: return spell_RF6_S_CYBER(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_CYBER */
    case RF_ABILITY::S_MONSTER: return spell_RF6_S_MONSTER(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_MONSTER */
    case RF_ABILITY::S_MONSTERS: return spell_RF6_S_MONSTERS(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_MONSTER */
    case RF_ABILITY::S_ANT: return spell_RF6_S_ANT(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_ANT */
    case RF_ABILITY::S_SPIDER: return spell_RF6_S_SPIDER(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_SPIDER */
    case RF_ABILITY::S_HOUND: return spell_RF6_S_HOUND(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_HOUND */
    case RF_ABILITY::S_HYDRA: return spell_RF6_S_HYDRA(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_HYDRA */
    case RF_ABILITY::S_ANGEL: return spell_RF6_S_ANGEL(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_ANGEL */
    case RF_ABILITY::S_DEMON: return spell_RF6_S_DEMON(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_DEMON */
    case RF_ABILITY::S_UNDEAD: return spell_RF6_S_UNDEAD(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_UNDEAD */
    case RF_ABILITY::S_DRAGON: return spell_RF6_S_DRAGON(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_DRAGON */
    case RF_ABILITY::S_HI_UNDEAD: return spell_RF6_S_HI_UNDEAD(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_HI_UNDEAD */
    case RF_ABILITY::S_HI_DRAGON: return spell_RF6_S_HI_DRAGON(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_HI_DRAGON */
    case RF_ABILITY::S_AMBERITES: return spell_RF6_S_AMBERITES(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_AMBERITES */
    case RF_ABILITY::S_UNIQUE: return spell_RF6_S_UNIQUE(target_ptr, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_S_UNIQUE */
    default: break;
    }
    // clang-format on

    return MonsterSpellResult::make_invalid();
}

static MonsterSpellResult monspell_to_monster_impl(
    player_type *target_ptr, RF_ABILITY ms_type, POSITION y, POSITION x, MONSTER_IDX m_idx, MONSTER_IDX t_idx, bool is_special_spell)
{
    // clang-format off
    switch (ms_type) {
    case RF_ABILITY::SHRIEK: return spell_RF4_SHRIEK(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF4_SHRIEK */
    case RF_ABILITY::XXX1: break;   /* RF4_XXX1 */
    case RF_ABILITY::DISPEL: return spell_RF4_DISPEL(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF4_DISPEL */
    case RF_ABILITY::ROCKET: return spell_RF4_ROCKET(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_ROCKET */
    case RF_ABILITY::SHOOT: return spell_RF4_SHOOT(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_SHOOT */
    case RF_ABILITY::XXX2: break;   /* RF4_XXX2 */
    case RF_ABILITY::XXX3: break;   /* RF4_XXX3 */
    case RF_ABILITY::XXX4: break;   /* RF4_XXX4 */
    case RF_ABILITY::BR_ACID: return spell_RF4_BREATH(target_ptr, GF_ACID, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_ACID */
    case RF_ABILITY::BR_ELEC: return spell_RF4_BREATH(target_ptr, GF_ELEC, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_ELEC */
    case RF_ABILITY::BR_FIRE: return spell_RF4_BREATH(target_ptr, GF_FIRE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_FIRE */
    case RF_ABILITY::BR_COLD: return spell_RF4_BREATH(target_ptr, GF_COLD, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_COLD */
    case RF_ABILITY::BR_POIS: return spell_RF4_BREATH(target_ptr, GF_POIS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_POIS */
    case RF_ABILITY::BR_NETH: return spell_RF4_BREATH(target_ptr, GF_NETHER, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_NETH */
    case RF_ABILITY::BR_LITE: return spell_RF4_BREATH(target_ptr, GF_LITE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_LITE */
    case RF_ABILITY::BR_DARK: return spell_RF4_BREATH(target_ptr, GF_DARK, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_DARK */
    case RF_ABILITY::BR_CONF: return spell_RF4_BREATH(target_ptr, GF_CONFUSION, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CONF */
    case RF_ABILITY::BR_SOUN: return spell_RF4_BREATH(target_ptr, GF_SOUND, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_SOUN */
    case RF_ABILITY::BR_CHAO: return spell_RF4_BREATH(target_ptr, GF_CHAOS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CHAO */
    case RF_ABILITY::BR_DISE: return spell_RF4_BREATH(target_ptr, GF_DISENCHANT, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_DISE */
    case RF_ABILITY::BR_NEXU: return spell_RF4_BREATH(target_ptr, GF_NEXUS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_NEXU */
    case RF_ABILITY::BR_TIME: return spell_RF4_BREATH(target_ptr, GF_TIME, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_TIME */
    case RF_ABILITY::BR_INER: return spell_RF4_BREATH(target_ptr, GF_INERTIAL, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_INER */
    case RF_ABILITY::BR_GRAV: return spell_RF4_BREATH(target_ptr, GF_GRAVITY, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_GRAV */
    case RF_ABILITY::BR_SHAR: return spell_RF4_BREATH(target_ptr, GF_SHARDS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_SHAR */
    case RF_ABILITY::BR_PLAS: return spell_RF4_BREATH(target_ptr, GF_PLASMA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_PLAS */
    case RF_ABILITY::BR_FORC: return spell_RF4_BREATH(target_ptr, GF_FORCE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_WALL */
    case RF_ABILITY::BR_MANA: return spell_RF4_BREATH(target_ptr, GF_MANA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_MANA */
    case RF_ABILITY::BA_NUKE: return spell_RF4_BA_NUKE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BA_NUKE */
    case RF_ABILITY::BR_NUKE: return spell_RF4_BREATH(target_ptr, GF_NUKE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_NUKE */
    case RF_ABILITY::BA_CHAO: return spell_RF4_BA_CHAO(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BA_CHAO */
    case RF_ABILITY::BR_DISI: return spell_RF4_BREATH(target_ptr, GF_DISINTEGRATE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_DISI */
    case RF_ABILITY::BA_ACID: return spell_RF5_BA_ACID(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_ACID */
    case RF_ABILITY::BA_ELEC: return spell_RF5_BA_ELEC(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_ELEC */
    case RF_ABILITY::BA_FIRE: return spell_RF5_BA_FIRE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_FIRE */
    case RF_ABILITY::BA_COLD: return spell_RF5_BA_COLD(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_COLD */
    case RF_ABILITY::BA_POIS: return spell_RF5_BA_POIS(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_POIS */
    case RF_ABILITY::BA_NETH: return spell_RF5_BA_NETH(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_NETH */
    case RF_ABILITY::BA_WATE: return spell_RF5_BA_WATE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_WATE */
    case RF_ABILITY::BA_MANA: return spell_RF5_BA_MANA(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_MANA */
    case RF_ABILITY::BA_DARK: return spell_RF5_BA_DARK(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_DARK */
    case RF_ABILITY::DRAIN_MANA: return spell_RF5_DRAIN_MANA(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_DRAIN_MANA */
    case RF_ABILITY::MIND_BLAST: return spell_RF5_MIND_BLAST(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_MIND_BLAST */
    case RF_ABILITY::BRAIN_SMASH: return spell_RF5_BRAIN_SMASH(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BRAIN_SMASH */
    case RF_ABILITY::CAUSE_1: return spell_RF5_CAUSE_1(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_CAUSE_1 */
    case RF_ABILITY::CAUSE_2: return spell_RF5_CAUSE_2(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_CAUSE_2 */
    case RF_ABILITY::CAUSE_3: return spell_RF5_CAUSE_3(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_CAUSE_3 */
    case RF_ABILITY::CAUSE_4: return spell_RF5_CAUSE_4(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_CAUSE_4 */
    case RF_ABILITY::BO_ACID: return spell_RF5_BO_ACID(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_ACID */
    case RF_ABILITY::BO_ELEC: return spell_RF5_BO_ELEC(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_ELEC */
    case RF_ABILITY::BO_FIRE: return spell_RF5_BO_FIRE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_FIRE */
    case RF_ABILITY::BO_COLD: return spell_RF5_BO_COLD(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_COLD */
    case RF_ABILITY::BA_LITE: return spell_RF5_BA_LITE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BA_LITE */
    case RF_ABILITY::BO_NETH: return spell_RF5_BO_NETH(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_NETH */
    case RF_ABILITY::BO_WATE: return spell_RF5_BO_WATE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_WATE */
    case RF_ABILITY::BO_MANA: return spell_RF5_BO_MANA(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_MANA */
    case RF_ABILITY::BO_PLAS: return spell_RF5_BO_PLAS(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_PLAS */
    case RF_ABILITY::BO_ICEE: return spell_RF5_BO_ICEE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_BO_ICEE */
    case RF_ABILITY::MISSILE: return spell_RF5_MISSILE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_MISSILE */
    case RF_ABILITY::SCARE: return spell_RF5_SCARE(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF5_SCARE */
    case RF_ABILITY::BLIND: return spell_RF5_BLIND(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF5_BLIND */
    case RF_ABILITY::CONF: return spell_RF5_CONF(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF5_CONF */
    case RF_ABILITY::SLOW: return spell_RF5_SLOW(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF5_SLOW */
    case RF_ABILITY::HOLD: return spell_RF5_HOLD(m_idx, target_ptr, t_idx, MONSTER_TO_MONSTER); /* RF5_HOLD */
    case RF_ABILITY::HASTE: return spell_RF6_HASTE(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_HASTE */
    case RF_ABILITY::HAND_DOOM: return spell_RF6_HAND_DOOM(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_HAND_DOOM */
    case RF_ABILITY::HEAL: return spell_RF6_HEAL(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_HEAL */
    case RF_ABILITY::INVULNER: return spell_RF6_INVULNER(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_INVULNER */
    case RF_ABILITY::BLINK: return spell_RF6_BLINK(target_ptr, m_idx, MONSTER_TO_MONSTER, is_special_spell); /* RF6_BLINK */
    case RF_ABILITY::TPORT: return spell_RF6_TPORT(target_ptr, m_idx, MONSTER_TO_MONSTER); /* RF6_TPORT */
    case RF_ABILITY::WORLD: break; /* RF6_WORLD */
    case RF_ABILITY::SPECIAL: return spell_RF6_SPECIAL(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF6_SPECIAL */
    case RF_ABILITY::TELE_TO: return spell_RF6_TELE_TO(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_TELE_TO */
    case RF_ABILITY::TELE_AWAY: return spell_RF6_TELE_AWAY(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_TELE_AWAY */
    case RF_ABILITY::TELE_LEVEL: return spell_RF6_TELE_LEVEL(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_TELE_LEVEL */
    case RF_ABILITY::PSY_SPEAR: return spell_RF6_PSY_SPEAR(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_PSY_SPEAR */
    case RF_ABILITY::DARKNESS: return spell_RF6_DARKNESS(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_DARKNESS */
    case RF_ABILITY::TRAPS: break; /* RF6_TRAPS */
    case RF_ABILITY::FORGET: break; /* RF6_FORGET */
    case RF_ABILITY::RAISE_DEAD: return spell_RF6_RAISE_DEAD(target_ptr, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_RAISE_DEAD */
    case RF_ABILITY::S_KIN: return spell_RF6_S_KIN(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_KIN */
    case RF_ABILITY::S_CYBER: return spell_RF6_S_CYBER(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_CYBER */
    case RF_ABILITY::S_MONSTER: return spell_RF6_S_MONSTER(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_MONSTER */
    case RF_ABILITY::S_MONSTERS: return spell_RF6_S_MONSTERS(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_MONSTER */
    case RF_ABILITY::S_ANT: return spell_RF6_S_ANT(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_ANT */
    case RF_ABILITY::S_SPIDER: return spell_RF6_S_SPIDER(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_SPIDER */
    case RF_ABILITY::S_HOUND: return spell_RF6_S_HOUND(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_HOUND */
    case RF_ABILITY::S_HYDRA: return spell_RF6_S_HYDRA(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_HYDRA */
    case RF_ABILITY::S_ANGEL: return spell_RF6_S_ANGEL(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_ANGEL */
    case RF_ABILITY::S_DEMON: return spell_RF6_S_DEMON(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_DEMON */
    case RF_ABILITY::S_UNDEAD: return spell_RF6_S_UNDEAD(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_UNDEAD */
    case RF_ABILITY::S_DRAGON: return spell_RF6_S_DRAGON(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_DRAGON */
    case RF_ABILITY::S_HI_UNDEAD: return spell_RF6_S_HI_UNDEAD(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_HI_UNDEAD */
    case RF_ABILITY::S_HI_DRAGON: return spell_RF6_S_HI_DRAGON(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_HI_DRAGON */
    case RF_ABILITY::S_AMBERITES: return spell_RF6_S_AMBERITES(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_AMBERITES */
    case RF_ABILITY::S_UNIQUE: return spell_RF6_S_UNIQUE(target_ptr, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_S_UNIQUE */
    default: break;
    }
    // clang-format on

    return MonsterSpellResult::make_invalid();
}

/*!
 * @brief モンスターからプレイヤーへの魔法使用。ラーニング処理も行う。
 * @param ms_type モンスター魔法ID (monster_spell_typeのenum値とは異なる)
 * @param y 対象の地点のy座標
 * @param x 対象の地点のx座標
 * @param m_idx 呪文を唱えるモンスターID
 */
MonsterSpellResult monspell_to_player(player_type *target_ptr, RF_ABILITY ms_type, POSITION y, POSITION x, MONSTER_IDX m_idx)
{
    // 特技使用前の時点でプレイヤーがモンスターを視認できているかチェック(ラーニングの必要条件)。
    const bool player_could_see_monster = spell_learnable(target_ptr, m_idx);

    auto res = monspell_to_player_impl(target_ptr, ms_type, y, x, m_idx);
    if (!player_could_see_monster)
        res.learnable = false;

    // 条件を満たしていればラーニングを試みる。
    if (res.valid && res.learnable) {
        const auto monspell = spell_idx_to_monster_spell_type(ms_type);
        learn_spell(target_ptr, monspell);
    }

    return res;
}

/*!
 * @brief モンスターからモンスターへの魔法使用。ラーニング処理も行う。
 * @param target_ptr プレーヤーへの参照ポインタ (monster_spell_typeのenum値とは異なる)
 * @param ms_type モンスター魔法ID
 * @param y 対象の地点のy座標
 * @param x 対象の地点のx座標
 * @param m_idx 呪文を唱えるモンスターID
 * @param t_idx 呪文を受けるモンスターID。プレイヤーの場合はdummyで0とする。
 * @param is_special_spell 特殊な行動である時TRUE
 * @todo モンスターからモンスターへの呪文なのにplayer_typeが引数になり得るのは間違っている……
 */
MonsterSpellResult monspell_to_monster(
    player_type *target_ptr, RF_ABILITY ms_type, POSITION y, POSITION x, MONSTER_IDX m_idx, MONSTER_IDX t_idx, bool is_special_spell)
{
    // 特技使用前の時点でプレイヤーがモンスターを視認できているかチェック(ラーニングの必要条件)。
    const bool player_could_see_monster = spell_learnable(target_ptr, m_idx);

    auto res = monspell_to_monster_impl(target_ptr, ms_type, y, x, m_idx, t_idx, is_special_spell);
    if (!player_could_see_monster)
        res.learnable = false;

    // 条件を満たしていればラーニングを試みる。
    if (res.valid && res.learnable) {
        const auto monspell = spell_idx_to_monster_spell_type(ms_type);
        learn_spell(target_ptr, monspell);
    }

    return res;
}
