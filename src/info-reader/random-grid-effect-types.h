﻿#pragma once

/* Random dungeon grid effects */
enum rdge_type {
    RANDOM_NONE = 0x00000000,
    RANDOM_FEATURE = 0x00000001,
    RANDOM_MONSTER = 0x00000002,
    RANDOM_OBJECT = 0x00000004,
    RANDOM_EGO = 0x00000008,
    RANDOM_ARTIFACT = 0x00000010,
    RANDOM_TRAP = 0x00000020,
};
