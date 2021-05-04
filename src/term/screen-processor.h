﻿#pragma once

#include "system/angband.h"

/** 画面情報保存スタックからの読み出しオプション */
enum class SCREEN_LOAD_OPT {
    ONE, //!< スタックの先頭のデータを1つ読み出す
    ALL, //!< スタックからすべてのデータを読み出す(画面は最後に読み出したデータになる)
};

void move_cursor(int row, int col);
void flush(void);
void screen_save();
void screen_load(SCREEN_LOAD_OPT opt = SCREEN_LOAD_OPT::ONE);
void c_put_str(TERM_COLOR attr, concptr str, TERM_LEN row, TERM_LEN col);
void put_str(concptr str, TERM_LEN row, TERM_LEN col);
void c_prt(TERM_COLOR attr, concptr str, TERM_LEN row, TERM_LEN col);
void prt(concptr str, TERM_LEN row, TERM_LEN col);
void c_roff(TERM_COLOR attr, concptr str);
void roff(concptr str);
void clear_from(int row);
