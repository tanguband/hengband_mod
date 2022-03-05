﻿/*!
 * @brief Index of spell type names
 */

#include "io/gf-descriptions.h"
#include "effect/attribute-types.h"

//clang-format off
const std::vector<named_num> gf_descriptions = {
    { "GF_ELEC", AttributeType::ELEC },
    { "GF_POIS", AttributeType::POIS },
    { "GF_ACID", AttributeType::ACID },
    { "GF_COLD", AttributeType::COLD },
    { "GF_FIRE", AttributeType::FIRE },
    { "GF_PSY_SPEAR", AttributeType::PSY_SPEAR },
    { "GF_MISSILE", AttributeType::MISSILE },
    { "GF_DEBUG", AttributeType::DEBUG },
    { "GF_PLASMA", AttributeType::PLASMA },
    { "GF_WATER", AttributeType::WATER },
    { "GF_LITE", AttributeType::LITE },
    { "GF_DARK", AttributeType::DARK },
    { "GF_LITE_WEAK", AttributeType::LITE_WEAK },
    { "GF_DARK_WEAK", AttributeType::DARK_WEAK },
    { "GF_SHARDS", AttributeType::SHARDS },
    { "GF_SOUND", AttributeType::SOUND },
    { "GF_CONFUSION", AttributeType::CONFUSION },
    { "GF_FORCE", AttributeType::FORCE },
    { "GF_INERTIA", AttributeType::INERTIAL },
    { "GF_MANA", AttributeType::MANA },
    { "GF_METEOR", AttributeType::METEOR },
    { "GF_ICE", AttributeType::ICE },
    { "GF_CHAOS", AttributeType::CHAOS },
    { "GF_NETHER", AttributeType::NETHER },
    { "GF_DISENCHANT", AttributeType::DISENCHANT },
    { "GF_NEXUS", AttributeType::NEXUS },
    { "GF_TIME", AttributeType::TIME },
    { "GF_GRAVITY", AttributeType::GRAVITY },
    { "GF_KILL_WALL", AttributeType::KILL_WALL },
    { "GF_KILL_DOOR", AttributeType::KILL_DOOR },
    { "GF_KILL_TRAP", AttributeType::KILL_TRAP },
    { "GF_MAKE_WALL", AttributeType::MAKE_WALL },
    { "GF_MAKE_DOOR", AttributeType::MAKE_DOOR },
    { "GF_MAKE_TRAP", AttributeType::MAKE_TRAP },
    { "GF_MAKE_TREE", AttributeType::MAKE_TREE },
    { "GF_OLD_CLONE", AttributeType::OLD_CLONE },
    { "GF_OLD_POLY", AttributeType::OLD_POLY },
    { "GF_OLD_HEAL", AttributeType::OLD_HEAL },
    { "GF_OLD_SPEED", AttributeType::OLD_SPEED },
    { "GF_OLD_SLOW", AttributeType::OLD_SLOW },
    { "GF_OLD_CONF", AttributeType::OLD_CONF },
    { "GF_OLD_SLEEP", AttributeType::OLD_SLEEP },
    { "GF_HYPODYNAMIA", AttributeType::HYPODYNAMIA },
    { "GF_AWAY_UNDEAD", AttributeType::AWAY_UNDEAD },
    { "GF_AWAY_EVIL", AttributeType::AWAY_EVIL },
    { "GF_AWAY_ALL", AttributeType::AWAY_ALL },
    { "GF_TURN_UNDEAD", AttributeType::TURN_UNDEAD },
    { "GF_TURN_EVIL", AttributeType::TURN_EVIL },
    { "GF_TURN_ALL", AttributeType::TURN_ALL },
    { "GF_DISP_UNDEAD", AttributeType::DISP_UNDEAD },
    { "GF_DISP_EVIL", AttributeType::DISP_EVIL },
    { "GF_DISP_ALL", AttributeType::DISP_ALL },
    { "GF_DISP_DEMON", AttributeType::DISP_DEMON },
    { "GF_DISP_LIVING", AttributeType::DISP_LIVING },
    { "GF_ROCKET", AttributeType::ROCKET },
    { "GF_NUKE", AttributeType::NUKE },
    { "GF_MAKE_RUNE_PROTECTION", AttributeType::MAKE_RUNE_PROTECTION },
    { "GF_STASIS", AttributeType::STASIS },
    { "GF_STONE_WALL", AttributeType::STONE_WALL },
    { "GF_DEATH_RAY", AttributeType::DEATH_RAY },
    { "GF_STUN", AttributeType::STUN },
    { "GF_HOLY_FIRE", AttributeType::HOLY_FIRE },
    { "GF_HELL_FIRE", AttributeType::HELL_FIRE },
    { "GF_DISINTEGRATE", AttributeType::DISINTEGRATE },
    { "GF_CHARM", AttributeType::CHARM },
    { "GF_CONTROL_UNDEAD", AttributeType::CONTROL_UNDEAD },
    { "GF_CONTROL_ANIMAL", AttributeType::CONTROL_ANIMAL },
    { "GF_PSI", AttributeType::PSI },
    { "GF_PSI_DRAIN", AttributeType::PSI_DRAIN },
    { "GF_TELEKINESIS", AttributeType::TELEKINESIS },
    { "GF_JAM_DOOR", AttributeType::JAM_DOOR },
    { "GF_DOMINATION", AttributeType::DOMINATION },
    { "GF_DISP_GOOD", AttributeType::DISP_GOOD },
    { "GF_DRAIN_MANA", AttributeType::DRAIN_MANA },
    { "GF_MIND_BLAST", AttributeType::MIND_BLAST },
    { "GF_BRAIN_SMASH", AttributeType::BRAIN_SMASH },
    { "GF_CAUSE_1", AttributeType::CAUSE_1 },
    { "GF_CAUSE_2", AttributeType::CAUSE_2 },
    { "GF_CAUSE_3", AttributeType::CAUSE_3 },
    { "GF_CAUSE_4", AttributeType::CAUSE_4 },
    { "GF_HAND_DOOM", AttributeType::HAND_DOOM },
    { "GF_CAPTURE", AttributeType::CAPTURE },
    { "GF_ANIM_DEAD", AttributeType::ANIM_DEAD },
    { "GF_CHARM_LIVING", AttributeType::CHARM_LIVING },
    { "GF_IDENTIFY", AttributeType::IDENTIFY },
    { "GF_ATTACK", AttributeType::ATTACK },
    { "GF_ENGETSU", AttributeType::ENGETSU },
    { "GF_GENOCIDE", AttributeType::GENOCIDE },
    { "GF_PHOTO", AttributeType::PHOTO },
    { "GF_CONTROL_DEMON", AttributeType::CONTROL_DEMON },
    { "GF_LAVA_FLOW", AttributeType::LAVA_FLOW },
    { "GF_BLOOD_CURSE", AttributeType::BLOOD_CURSE },
    { "GF_SEEKER", AttributeType::SEEKER },
    { "GF_SUPER_RAY", AttributeType::SUPER_RAY },
    { "GF_STAR_HEAL", AttributeType::STAR_HEAL },
    { "GF_WATER_FLOW", AttributeType::WATER_FLOW },
    { "GF_CRUSADE", AttributeType::CRUSADE },
    { "GF_STASIS_EVIL", AttributeType::STASIS_EVIL },
    { "GF_WOUNDS", AttributeType::WOUNDS },
    { "GF_E_GENOCIDE", AttributeType::E_GENOCIDE },
    { "GF_VOID", AttributeType::VOID_MAGIC },
    { "GF_ABYSS", AttributeType::ABYSS },
    { "GF_HUNGRY", AttributeType::HUNGRY },
    { "GF_MONSTER_SHOOT", AttributeType::MONSTER_SHOOT },
    { "GF_MONSTER_MELEE", AttributeType::MONSTER_MELEE },
};

//clang-format on
