﻿#include "target/grid-selector.h"
#include "core/player-redraw-types.h"
#include "core/player-update-types.h"
#include "core/stuff-handler.h"
#include "core/window-redrawer.h"
#include "floor/cave.h"
#include "floor/geometry.h"
#include "game-option/game-play-options.h"
#include "game-option/input-options.h"
#include "game-option/keymap-directory-getter.h"
#include "grid/feature-flag-types.h"
#include "grid/feature.h"
#include "io/cursor.h"
#include "io/input-key-acceptor.h"
#include "io/screen-util.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "target/target-checker.h"
#include "term/screen-processor.h"
#include "util/int-char-converter.h"
#include "util/sort.h"
#include "view/display-messages.h"
#include "window/main-window-util.h"
#include <unordered_map>
#include <vector>

/*
 * XAngband: determine if a given location is "interesting"
 * based on target_set_accept function.
 */
static bool tgt_pt_accept(player_type *creature_ptr, POSITION y, POSITION x)
{
    floor_type *floor_ptr = creature_ptr->current_floor_ptr;
    if (!(in_bounds(floor_ptr, y, x)))
        return false;

    if ((y == creature_ptr->y) && (x == creature_ptr->x))
        return true;

    if (creature_ptr->image)
        return false;

    grid_type *g_ptr;
    g_ptr = &floor_ptr->grid_array[y][x];
    if (!g_ptr->is_mark())
        return false;

    if (g_ptr->cave_has_flag(FF_LESS) || g_ptr->cave_has_flag(FF_MORE) || g_ptr->cave_has_flag(FF_QUEST_ENTER)
        || g_ptr->cave_has_flag(FF_QUEST_EXIT))
        return true;

    if (g_ptr->cave_has_flag(FF_STORE) || g_ptr->cave_has_flag(FF_BLDG))
        return true;

    return false;
}

/*
 * XAngband: Prepare the "temp" array for "tget_pt"
 * based on target_set_prepare funciton.
 */
static void tgt_pt_prepare(player_type *creature_ptr, std::vector<POSITION> &ys, std::vector<POSITION> &xs)
{
    if (!expand_list)
        return;

    floor_type *floor_ptr = creature_ptr->current_floor_ptr;
    for (POSITION y = 1; y < floor_ptr->height; y++) {
        for (POSITION x = 1; x < floor_ptr->width; x++) {
            if (!tgt_pt_accept(creature_ptr, y, x))
                continue;

            ys.emplace_back(y);
            xs.emplace_back(x);
        }
    }

    ang_sort(creature_ptr, xs.data(), ys.data(), size(ys), ang_sort_comp_distance, ang_sort_swap_position);
}

/*!
 * @brief グリッドのシンボルが指定した記号かどうかを調べる
 * @param g_ptr グリッド情報への参照ポインタ
 * @param ch 指定するシンボル文字
 * @return シンボルが指定した記号ならTRUE、そうでなければFALSE
 */
static bool cave_is_symbol_grid(grid_type *g_ptr, char ch)
{
    return f_info[g_ptr->feat].x_char[0] == ch;
}

/*!
 * @brief 指定したシンボルのマスかどうかを判定するための条件式コールバック
 */
std::unordered_map<int, std::function<bool(grid_type *)>> tgt_pt_symbol_call_back = {
    { '<', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STAIRS) && g_ptr->cave_has_flag(FF_LESS); } },
    { '>', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STAIRS) && g_ptr->cave_has_flag(FF_MORE); } },
    { '+', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_BLDG); } },
    { '0', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '0'); } },
    { '!', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '1'); } },
    { '"', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '2'); } },
    { '#', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '3'); } },
    { '$', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '4'); } },
    { '%', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '5'); } },
    { '&', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '6'); } },
    { '\'', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '7'); } },
    { '(', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '8'); } },
    { ')', [](grid_type *g_ptr) { return g_ptr->cave_has_flag(FF_STORE) && cave_is_symbol_grid(g_ptr, '9'); } },
};

/*!
 * @brief 位置ターゲット指定情報構造体
 * @details
 * ang_sort() を利用する関係上、y/x 座標それぞれについて配列を作る。
 */
struct tgt_pt_info {
    TERM_LEN wid; //!< 画面サイズ(幅)
    TERM_LEN hgt; //!< 画面サイズ(高さ)
    POSITION y; //!< 現在の指定位置(Y)
    POSITION x; //!< 現在の指定位置(X)
    std::vector<POSITION> ys; //!< "interesting" な座標たちを記録する配列(Y)
    std::vector<POSITION> xs; //!< "interesting" な座標たちを記録する配列(X)
    size_t n; //<! シンボル配列の何番目か
    char ch; //<! 入力キー
    char prev_ch; //<! 前回入力キー
    std::function<bool(grid_type *)> callback; //<! 条件判定コールバック
};

/*!
 * @brief 指定した記号のシンボルのグリッドにカーソルを移動する
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param info 位置ターゲット指定情報構造体(参照渡し)
 */
void tgt_pt_move_to_symbol(player_type *creature_ptr, tgt_pt_info &info)
{
    if (!expand_list || info.ys.empty())
        return;

    int dx, dy;
    int cx = (panel_col_min + panel_col_max) / 2;
    int cy = (panel_row_min + panel_row_max) / 2;
    if (info.ch != info.prev_ch)
        info.n = 0;
    info.prev_ch = info.ch;
    info.n++;

    for (; info.n < size(info.ys); ++info.n) {
        const POSITION y_cur = info.ys[info.n];
        const POSITION x_cur = info.xs[info.n];
        grid_type *g_ptr = &creature_ptr->current_floor_ptr->grid_array[y_cur][x_cur];
        if (info.callback(g_ptr))
            break;
    }

    if (info.n == size(info.ys)) {
        info.n = 0;
        info.y = creature_ptr->y;
        info.x = creature_ptr->x;
        verify_panel(creature_ptr);
        creature_ptr->update |= PU_MONSTERS;
        creature_ptr->redraw |= PR_MAP;
        creature_ptr->window_flags |= PW_OVERHEAD;
        handle_stuff(creature_ptr);
    } else {
        info.y = info.ys[info.n];
        info.x = info.xs[info.n];
        dy = 2 * (info.y - cy) / info.hgt;
        dx = 2 * (info.x - cx) / info.wid;
        if (dy || dx)
            change_panel(creature_ptr, dy, dx);
    }
}

/*!
 * @brief 位置を指定するプロンプト
 * @param creature_ptr プレイヤー情報への参照ポインタ
 * @param x_ptr x座標への参照ポインタ
 * @param y_ptr y座標への参照ポインタ
 * @return 指定したらTRUE、キャンセルしたらFALSE
 */
bool tgt_pt(player_type *creature_ptr, POSITION *x_ptr, POSITION *y_ptr)
{
    tgt_pt_info info;
    get_screen_size(&info.wid, &info.hgt);

    info.y = creature_ptr->y;
    info.x = creature_ptr->x;
    if (expand_list)
        tgt_pt_prepare(creature_ptr, info.ys, info.xs);

    msg_print(_("場所を選んでスペースキーを押して下さい。", "Select a point and press space."));
    msg_flag = false;

    info.ch = 0;
    info.n = 0;
    bool success = false;
    while ((info.ch != ESCAPE) && !success) {
        bool move_fast = false;
        move_cursor_relative(info.y, info.x);
        info.ch = inkey();
        switch (info.ch) {
        case ESCAPE:
            break;
        case ' ':
        case 't':
        case '.':
            if (player_bold(creature_ptr, info.y, info.x))
                info.ch = 0;
            else
                success = true;
            break;
        case '>':
        case '<':
        case '+':
        case '!':
        case '"':
        case '#':
        case '$':
        case '%':
        case '&':
        case '\'':
        case '(':
        case ')': {
            info.callback = tgt_pt_symbol_call_back[info.ch];
            tgt_pt_move_to_symbol(creature_ptr, info);
            break;
        }
        default: {
            if (rogue_like_commands) {
                if (info.ch >= '0' && info.ch <= '9') {
                    if (info.ch != '0')
                        info.ch -= 16;
                    info.callback = tgt_pt_symbol_call_back[info.ch];
                    tgt_pt_move_to_symbol(creature_ptr, info);
                    break;
                }
            } else {
                if (info.ch == '5' || info.ch == '0') {
                    if (player_bold(creature_ptr, info.y, info.x))
                        info.ch = 0;
                    else
                        success = true;
                    break;
                }
            }

            int d = get_keymap_dir(info.ch);
            if (isupper(info.ch))
                move_fast = true;

            if (d == 0)
                break;

            int dx = ddx[d];
            int dy = ddy[d];
            if (move_fast) {
                int mag = MIN(info.wid / 2, info.hgt / 2);
                info.x += dx * mag;
                info.y += dy * mag;
            } else {
                info.x += dx;
                info.y += dy;
            }

            if (((info.x < panel_col_min + info.wid / 2) && (dx > 0)) || ((info.x > panel_col_min + info.wid / 2) && (dx < 0)))
                dx = 0;

            if (((info.y < panel_row_min + info.hgt / 2) && (dy > 0)) || ((info.y > panel_row_min + info.hgt / 2) && (dy < 0)))
                dy = 0;

            if ((info.y >= panel_row_min + info.hgt) || (info.y < panel_row_min) || (info.x >= panel_col_min + info.wid) || (info.x < panel_col_min))
                change_panel(creature_ptr, dy, dx);

            if (info.x >= creature_ptr->current_floor_ptr->width - 1)
                info.x = creature_ptr->current_floor_ptr->width - 2;
            else if (info.x <= 0)
                info.x = 1;

            if (info.y >= creature_ptr->current_floor_ptr->height - 1)
                info.y = creature_ptr->current_floor_ptr->height - 2;
            else if (info.y <= 0)
                info.y = 1;

            break;
        }
        }
    }

    prt("", 0, 0);
    verify_panel(creature_ptr);
    creature_ptr->update |= PU_MONSTERS;
    creature_ptr->redraw |= PR_MAP;
    creature_ptr->window_flags |= PW_OVERHEAD;
    handle_stuff(creature_ptr);
    *x_ptr = info.x;
    *y_ptr = info.y;
    return success;
}
