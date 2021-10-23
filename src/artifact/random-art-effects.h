﻿#pragma once

/*!
 * @file random-art-effects.h
 * @brief ランダムアーティファクトの発動ID定義
 * @details Random Artifact Activation Type
 */
enum class RandomArtActType : short {
    NONE = 0,
    SUNLIGHT = 1,
    BO_MISS_1 = 2,
    BA_POIS_1 = 3,
    BO_ELEC_1 = 4,
    BO_ACID_1 = 5,
    BO_COLD_1 = 6,
    BO_FIRE_1 = 7,
    BA_COLD_1 = 8,
    BA_FIRE_1 = 9,
    HYPODYNAMIA_1 = 10,
    BA_COLD_2 = 11,
    BA_ELEC_2 = 12,
    HYPODYNAMIA_2 = 13,
    DRAIN_1 = 14,
    BO_MISS_2 = 15,
    BA_FIRE_3 = 16,
    BA_COLD_3 = 17,
    BA_ELEC_3 = 18,
    WHIRLWIND = 19,
    DRAIN_2 = 20,
    CALL_CHAOS = 21,
    ROCKET = 22,
    DISP_EVIL = 23,
    BA_MISS_3 = 24,
    DISP_GOOD = 25,
    BO_MANA = 26,
    BA_FIRE_2 = 27,
    BA_WATER = 28,
    BA_STAR = 29,
    BA_DARK = 30,
    BA_MANA = 31,
    PESTICIDE = 32,
    BLINDING_LIGHT = 33,
    BIZARRE = 34,
    CAST_BA_STAR = 35,
    BLADETURNER = 36,
    BA_ACID_1 = 37,
    BR_FIRE = 38,
    BR_COLD = 39,
    BR_DRAGON = 40,
    BA_FIRE_4 = 41,
    BA_NUKE_1 = 42,
    /* 43 - 48 unused */
    TREE_CREATION = 49,
    ANIM_DEAD = 50,
    CONFUSE = 51,
    SLEEP = 52,
    QUAKE = 53,
    TERROR = 54,
    TELE_AWAY = 55,
    BANISH_EVIL = 56,
    GENOCIDE = 57,
    MASS_GENO = 58,
    SCARE_AREA = 59,
    AGGRAVATE = 60,
    /* 59 - 64 unused */
    CHARM_ANIMAL = 65,
    CHARM_UNDEAD = 66,
    CHARM_OTHER = 67,
    CHARM_ANIMALS = 68,
    CHARM_OTHERS = 69,
    SUMMON_ANIMAL = 70,
    SUMMON_PHANTOM = 71,
    SUMMON_ELEMENTAL = 72,
    SUMMON_DEMON = 73,
    SUMMON_UNDEAD = 74,
    SUMMON_HOUND = 75,
    SUMMON_DAWN = 76,
    SUMMON_OCTOPUS = 77,
    /* 78 - 79 unused */
    CHOIR_SINGS = 80,
    CURE_LW = 81,
    CURE_MW = 82,
    CURE_POISON = 83,
    REST_EXP = 84,
    REST_ALL = 85,
    CURE_700 = 86,
    CURE_1000 = 87,
    CURING = 88,
    CURE_MANA_FULL = 89,
    ESP = 91,
    BERSERK = 92,
    PROT_EVIL = 93,
    RESIST_ALL = 94,
    SPEED = 95,
    XTRA_SPEED = 96,
    WRAITH = 97,
    INVULN = 98,
    HERO = 99,
    HERO_SPEED = 100,
    ACID_BALL_AND_RESISTANCE = 101,
    FIRE_BALL_AND_RESISTANCE = 102,
    COLD_BALL_AND_RESISTANCE = 103,
    ELEC_BALL_AND_RESISTANCE = 104,
    POIS_BALL_AND_RESISTANCE = 105,
    RESIST_ACID = 106,
    RESIST_FIRE = 107,
    RESIST_COLD = 108,
    RESIST_ELEC = 109,
    RESIST_POIS = 110,
    LIGHT = 111,
    MAP_LIGHT = 112,
    DETECT_ALL = 113,
    DETECT_XTRA = 114,
    ID_FULL = 115,
    ID_PLAIN = 116,
    RUNE_EXPLO = 117,
    RUNE_PROT = 118,
    SATIATE = 119,
    DEST_DOOR = 120,
    STONE_MUD = 121,
    RECHARGE = 122,
    ALCHEMY = 123,
    DIM_DOOR = 124,
    TELEPORT = 125,
    RECALL = 126,
    JUDGE = 127,
    TELEKINESIS = 128,
    DETECT_UNIQUE = 129,
    ESCAPE = 130,
    DISP_CURSE_XTRA = 131,
    BRAND_FIRE_BOLTS = 132,
    RECHARGE_XTRA = 133,
    LORE = 134,
    SHIKOFUMI = 135,
    PHASE_DOOR = 136,
    DETECT_ALL_MONS = 137,
    ULTIMATE_RESIST = 138,
    ELBERETH = 139,
    DETECT_TREASURE = 140,
    /* 141 - 245 unused */
    FALLING_STAR = 246,
    STRAIN_HASTE = 247,
    TELEPORT_LEVEL = 248,
    GRAND_CROSS = 249,
    CAST_OFF = 250,
    FISHING = 251,
    INROU = 252,
    MURAMASA = 253,
    BLOODY_MOON = 254,
    CRIMSON = 255,
    MAX,
};
