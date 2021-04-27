﻿#pragma once

#include <vector>

#include "system/angband.h"

#define MAX_GAME_INSCRIPTIONS 10

/*! オブジェクトの特性表示記号テーブルの構造体 / Structs and tables for Auto Inscription for flags */
typedef struct flag_insc_table {
#ifdef JP
    concptr japanese;
#endif
    concptr english;
    int flag;
    int except_flag;
} flag_insc_table;

extern const concptr game_inscriptions[MAX_GAME_INSCRIPTIONS];

extern std::vector<flag_insc_table>flag_insc_plus;
extern std::vector<flag_insc_table>flag_insc_immune;
extern std::vector<flag_insc_table> flag_insc_vuln;
extern std::vector<flag_insc_table> flag_insc_resistance;
extern std::vector<flag_insc_table> flag_insc_misc;
extern std::vector<flag_insc_table>flag_insc_aura;
extern std::vector<flag_insc_table>flag_insc_brand;
extern std::vector<flag_insc_table>flag_insc_kill;
extern std::vector<flag_insc_table>flag_insc_slay;
extern std::vector<flag_insc_table>flag_insc_esp1;
extern std::vector<flag_insc_table>flag_insc_esp2;
extern std::vector<flag_insc_table>flag_insc_sust;
