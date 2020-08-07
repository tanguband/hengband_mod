﻿#include "cmd-action/cmd-racial.h"
#include "action/action-limited.h"
#include "action/mutation-execution.h"
#include "core/asking-player.h"
#include "core/player-redraw-types.h"
#include "core/player-update-types.h"
#include "core/window-redrawer.h"
#include "game-option/text-display-options.h"
#include "io/command-repeater.h"
#include "io/input-key-requester.h"
#include "main/sound-of-music.h"
#include "mutation/mutation-flag-types.h"
#include "player/attack-defense-types.h"
#include "player/player-damage.h"
#include "player/special-defense-types.h"
#include "racial/class-racial-switcher.h"
#include "racial/race-racial-switcher.h"
#include "racial/racial-switcher.h"
#include "racial/racial-util.h"
#include "status/action-setter.h"
#include "term/screen-processor.h"
#include "util/int-char-converter.h"

/*!
 * @brief レイシャル・パワーコマンドのメインルーチン / Allow user to choose a power (racial / mutation) to activate
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @return なし
 */
void do_cmd_racial_power(player_type *creature_ptr)
{
    if (creature_ptr->wild_mode)
        return;

    if (cmd_limit_confused(creature_ptr)) {
        free_turn(creature_ptr);
        return;
    }

    if (creature_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN))
        set_action(creature_ptr, ACTION_NONE);

    rc_type tmp_rc;
    rc_type *rc_ptr = initialize_rc_type(creature_ptr, &tmp_rc);
    switch_class_racial(creature_ptr, rc_ptr);
    if (creature_ptr->mimic_form)
        switch_mimic_racial(creature_ptr, rc_ptr);
    else
        switch_race_racial(creature_ptr, rc_ptr);

    if (creature_ptr->muta1) {
        if (creature_ptr->muta1 & MUT1_SPIT_ACID) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("酸の唾", "Spit Acid"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 9;
            rc_ptr->power_desc[rc_ptr->num].cost = 9;
            rc_ptr->power_desc[rc_ptr->num].stat = A_DEX;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_SPIT_ACID;
        }

        if (creature_ptr->muta1 & MUT1_BR_FIRE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("炎のブレス", "Fire Breath"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 20;
            rc_ptr->power_desc[rc_ptr->num].cost = rc_ptr->lvl;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 18;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_BR_FIRE;
        }

        if (creature_ptr->muta1 & MUT1_HYPN_GAZE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("催眠睨み", "Hypnotic Gaze"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 12;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CHR;
            rc_ptr->power_desc[rc_ptr->num].fail = 18;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_HYPN_GAZE;
        }

        if (creature_ptr->muta1 & MUT1_TELEKINES) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("念動力", "Telekinesis"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 9;
            rc_ptr->power_desc[rc_ptr->num].cost = 9;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_TELEKINES;
        }

        if (creature_ptr->muta1 & MUT1_VTELEPORT) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("テレポート", "Teleport"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 7;
            rc_ptr->power_desc[rc_ptr->num].cost = 7;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_VTELEPORT;
        }

        if (creature_ptr->muta1 & MUT1_MIND_BLST) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("精神攻撃", "Mind Blast"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 5;
            rc_ptr->power_desc[rc_ptr->num].cost = 3;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_MIND_BLST;
        }

        if (creature_ptr->muta1 & MUT1_RADIATION) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("放射能", "Emit Radiation"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 15;
            rc_ptr->power_desc[rc_ptr->num].cost = 15;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_RADIATION;
        }

        if (creature_ptr->muta1 & MUT1_VAMPIRISM) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("吸血", "Vampiric Drain"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 2;
            rc_ptr->power_desc[rc_ptr->num].cost = (1 + (rc_ptr->lvl / 3));
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 9;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_VAMPIRISM;
        }

        if (creature_ptr->muta1 & MUT1_SMELL_MET) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("金属嗅覚", "Smell Metal"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 3;
            rc_ptr->power_desc[rc_ptr->num].cost = 2;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 12;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_SMELL_MET;
        }

        if (creature_ptr->muta1 & MUT1_SMELL_MON) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("敵臭嗅覚", "Smell Monsters"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 5;
            rc_ptr->power_desc[rc_ptr->num].cost = 4;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_SMELL_MON;
        }

        if (creature_ptr->muta1 & MUT1_BLINK) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("ショート・テレポート", "Blink"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 3;
            rc_ptr->power_desc[rc_ptr->num].cost = 3;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 12;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_BLINK;
        }

        if (creature_ptr->muta1 & MUT1_EAT_ROCK) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("岩食い", "Eat Rock"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 8;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 18;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_EAT_ROCK;
        }

        if (creature_ptr->muta1 & MUT1_SWAP_POS) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("位置交換", "Swap Position"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 15;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_DEX;
            rc_ptr->power_desc[rc_ptr->num].fail = 16;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_SWAP_POS;
        }

        if (creature_ptr->muta1 & MUT1_SHRIEK) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("叫び", "Shriek"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 20;
            rc_ptr->power_desc[rc_ptr->num].cost = 14;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 16;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_SHRIEK;
        }

        if (creature_ptr->muta1 & MUT1_ILLUMINE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("照明", "Illuminate"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 3;
            rc_ptr->power_desc[rc_ptr->num].cost = 2;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 10;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_ILLUMINE;
        }

        if (creature_ptr->muta1 & MUT1_DET_CURSE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("呪い感知", "Detect Curses"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 7;
            rc_ptr->power_desc[rc_ptr->num].cost = 14;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_DET_CURSE;
        }

        if (creature_ptr->muta1 & MUT1_BERSERK) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("狂戦士化", "Berserk"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 8;
            rc_ptr->power_desc[rc_ptr->num].cost = 8;
            rc_ptr->power_desc[rc_ptr->num].stat = A_STR;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_BERSERK;
        }

        if (creature_ptr->muta1 & MUT1_POLYMORPH) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("変身", "Polymorph"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 18;
            rc_ptr->power_desc[rc_ptr->num].cost = 20;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 18;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_POLYMORPH;
        }

        if (creature_ptr->muta1 & MUT1_MIDAS_TCH) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("ミダスの手", "Midas Touch"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 10;
            rc_ptr->power_desc[rc_ptr->num].cost = 5;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 12;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_MIDAS_TCH;
        }

        if (creature_ptr->muta1 & MUT1_GROW_MOLD) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("カビ発生", "Grow Mold"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 1;
            rc_ptr->power_desc[rc_ptr->num].cost = 6;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_GROW_MOLD;
        }

        if (creature_ptr->muta1 & MUT1_RESIST) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("エレメント耐性", "Resist Elements"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 10;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 12;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_RESIST;
        }

        if (creature_ptr->muta1 & MUT1_EARTHQUAKE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("地震", "Earthquake"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 12;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_STR;
            rc_ptr->power_desc[rc_ptr->num].fail = 16;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_EARTHQUAKE;
        }

        if (creature_ptr->muta1 & MUT1_EAT_MAGIC) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("魔力食い", "Eat Magic"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 17;
            rc_ptr->power_desc[rc_ptr->num].cost = 1;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_EAT_MAGIC;
        }

        if (creature_ptr->muta1 & MUT1_WEIGH_MAG) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("魔力感知", "Weigh Magic"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 6;
            rc_ptr->power_desc[rc_ptr->num].cost = 6;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 10;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_WEIGH_MAG;
        }

        if (creature_ptr->muta1 & MUT1_STERILITY) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("増殖阻止", "Sterilize"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 12;
            rc_ptr->power_desc[rc_ptr->num].cost = 23;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CHR;
            rc_ptr->power_desc[rc_ptr->num].fail = 15;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_STERILITY;
        }

        if (creature_ptr->muta1 & MUT1_HIT_AND_AWAY) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("ヒット＆アウェイ", "Panic Hit"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 10;
            rc_ptr->power_desc[rc_ptr->num].cost = 12;
            rc_ptr->power_desc[rc_ptr->num].stat = A_DEX;
            rc_ptr->power_desc[rc_ptr->num].fail = 14;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_HIT_AND_AWAY;
        }

        if (creature_ptr->muta1 & MUT1_DAZZLE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("眩惑", "Dazzle"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 7;
            rc_ptr->power_desc[rc_ptr->num].cost = 15;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CHR;
            rc_ptr->power_desc[rc_ptr->num].fail = 8;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_DAZZLE;
        }

        if (creature_ptr->muta1 & MUT1_LASER_EYE) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("レーザー・アイ", "Laser Eye"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 7;
            rc_ptr->power_desc[rc_ptr->num].cost = 10;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 9;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_LASER_EYE;
        }

        if (creature_ptr->muta1 & MUT1_RECALL) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("帰還", "Recall"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 17;
            rc_ptr->power_desc[rc_ptr->num].cost = 50;
            rc_ptr->power_desc[rc_ptr->num].stat = A_INT;
            rc_ptr->power_desc[rc_ptr->num].fail = 16;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_RECALL;
        }

        if (creature_ptr->muta1 & MUT1_BANISH) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("邪悪消滅", "Banish Evil"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 25;
            rc_ptr->power_desc[rc_ptr->num].cost = 25;
            rc_ptr->power_desc[rc_ptr->num].stat = A_WIS;
            rc_ptr->power_desc[rc_ptr->num].fail = 18;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_BANISH;
        }

        if (creature_ptr->muta1 & MUT1_COLD_TOUCH) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("凍結の手", "Cold Touch"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 2;
            rc_ptr->power_desc[rc_ptr->num].cost = 2;
            rc_ptr->power_desc[rc_ptr->num].stat = A_CON;
            rc_ptr->power_desc[rc_ptr->num].fail = 11;
            rc_ptr->power_desc[rc_ptr->num++].number = MUT1_COLD_TOUCH;
        }

        if (creature_ptr->muta1 & MUT1_LAUNCHER) {
            strcpy(rc_ptr->power_desc[rc_ptr->num].racial_name, _("アイテム投げ", "Throw Object"));
            rc_ptr->power_desc[rc_ptr->num].min_level = 1;
            rc_ptr->power_desc[rc_ptr->num].cost = rc_ptr->lvl;
            rc_ptr->power_desc[rc_ptr->num].stat = A_STR;
            rc_ptr->power_desc[rc_ptr->num].fail = 6;
            rc_ptr->power_desc[rc_ptr->num++].number = 3;
        }
    }

    rc_ptr->flag = FALSE;
    rc_ptr->redraw = FALSE;

    (void)strnfmt(rc_ptr->out_val, 78,
        _("(特殊能力 %c-%c, *'で一覧, ESCで中断) どの特殊能力を使いますか？", "(Powers %c-%c, *=List, ESC=exit) Use which power? "),
        I2A(0), (rc_ptr->num <= 26) ? I2A(rc_ptr->num - 1) : '0' + rc_ptr->num - 27);

    if (!repeat_pull(&rc_ptr->command_code) || rc_ptr->command_code < 0 || rc_ptr->command_code >= rc_ptr->num) {
        if (use_menu)
            screen_save();

        rc_ptr->choice = (always_show_list || use_menu) ? ESCAPE : 1;
        while (!rc_ptr->flag) {
            if (rc_ptr->choice == ESCAPE)
                rc_ptr->choice = ' ';
            else if (!get_com(rc_ptr->out_val, &rc_ptr->choice, FALSE))
                break;

            if (use_menu && rc_ptr->choice != ' ') {
                switch (rc_ptr->choice) {
                case '0': {
                    screen_load();
                    free_turn(creature_ptr);
                    return;
                }

                case '8':
                case 'k':
                case 'K': {
                    rc_ptr->menu_line += (rc_ptr->num - 1);
                    break;
                }

                case '2':
                case 'j':
                case 'J': {
                    rc_ptr->menu_line++;
                    break;
                }

                case '6':
                case 'l':
                case 'L':
                case '4':
                case 'h':
                case 'H': {
                    if (rc_ptr->menu_line > 18)
                        rc_ptr->menu_line -= 18;
                    else if (rc_ptr->menu_line + 18 <= rc_ptr->num)
                        rc_ptr->menu_line += 18;
                    break;
                }

                case 'x':
                case 'X':
                case '\r': {
                    rc_ptr->command_code = rc_ptr->menu_line - 1;
                    rc_ptr->ask = FALSE;
                    break;
                }
                }
                if (rc_ptr->menu_line > rc_ptr->num)
                    rc_ptr->menu_line -= rc_ptr->num;
            }

            if ((rc_ptr->choice == ' ') || (rc_ptr->choice == '*') || (rc_ptr->choice == '?') || (use_menu && rc_ptr->ask)) {
                if (!rc_ptr->redraw || use_menu) {
                    byte y = 1, x = 0;
                    int ctr = 0;
                    char dummy[80];
                    char letter;
                    TERM_LEN x1, y1;
                    strcpy(dummy, "");
                    rc_ptr->redraw = TRUE;
                    if (!use_menu)
                        screen_save();

                    if (rc_ptr->num < 18)
                        prt(_("                            Lv   MP 失率", "                            Lv Cost Fail"), y++, x);
                    else
                        prt(_("                            Lv   MP 失率                            Lv   MP 失率",
                                "                            Lv Cost Fail                            Lv Cost Fail"),
                            y++, x);

                    while (ctr < rc_ptr->num) {
                        x1 = ((ctr < 18) ? x : x + 40);
                        y1 = ((ctr < 18) ? y + ctr : y + ctr - 18);
                        if (use_menu) {
                            if (ctr == (rc_ptr->menu_line - 1))
                                strcpy(dummy, _(" 》 ", " >  "));
                            else
                                strcpy(dummy, "    ");
                        } else {
                            if (ctr < 26)
                                letter = I2A(ctr);
                            else
                                letter = '0' + ctr - 26;
                            sprintf(dummy, " %c) ", letter);
                        }

                        strcat(dummy,
                            format("%-23.23s %2d %4d %3d%%", rc_ptr->power_desc[ctr].racial_name, rc_ptr->power_desc[ctr].min_level, rc_ptr->power_desc[ctr].cost,
                                100 - racial_chance(creature_ptr, &rc_ptr->power_desc[ctr])));
                        prt(dummy, y1, x1);
                        ctr++;
                    }
                } else {
                    rc_ptr->redraw = FALSE;
                    screen_load();
                }

                continue;
            }

            if (!use_menu) {
                if (rc_ptr->choice == '\r' && rc_ptr->num == 1)
                    rc_ptr->choice = 'a';

                if (isalpha(rc_ptr->choice)) {
                    rc_ptr->ask = (isupper(rc_ptr->choice));
                    if (rc_ptr->ask)
                        rc_ptr->choice = (char)tolower(rc_ptr->choice);

                    rc_ptr->command_code = (islower(rc_ptr->choice) ? A2I(rc_ptr->choice) : -1);
                } else {
                    rc_ptr->ask = FALSE;
                    rc_ptr->command_code = rc_ptr->choice - '0' + 26;
                }
            }

            if ((rc_ptr->command_code < 0) || (rc_ptr->command_code >= rc_ptr->num)) {
                bell();
                continue;
            }

            if (rc_ptr->ask) {
                char tmp_val[160];
                (void)strnfmt(tmp_val, 78, _("%sを使いますか？ ", "Use %s? "), rc_ptr->power_desc[rc_ptr->command_code].racial_name);
                if (!get_check(tmp_val))
                    continue;
            }

            rc_ptr->flag = TRUE;
        }

        if (rc_ptr->redraw)
            screen_load();

        if (!rc_ptr->flag) {
            free_turn(creature_ptr);
            return;
        }

        repeat_push(rc_ptr->command_code);
    }

    switch (racial_aux(creature_ptr, &rc_ptr->power_desc[rc_ptr->command_code])) {
    case 1:
        if (rc_ptr->power_desc[rc_ptr->command_code].number < 0)
            rc_ptr->cast = exe_racial_power(creature_ptr, rc_ptr->power_desc[rc_ptr->command_code].number);
        else
            rc_ptr->cast = exe_mutation_power(creature_ptr, rc_ptr->power_desc[rc_ptr->command_code].number);
        break;
    case 0:
        rc_ptr->cast = FALSE;
        break;
    case -1:
        rc_ptr->cast = TRUE;
        break;
    }

    if (!rc_ptr->cast) {
        free_turn(creature_ptr);
        return;
    }

    int racial_cost = rc_ptr->power_desc[rc_ptr->command_code].racial_cost;
    if (racial_cost == 0)
        return;

    int actual_racial_cost = racial_cost / 2 + randint1(racial_cost / 2);
    if (creature_ptr->csp < actual_racial_cost) {
        actual_racial_cost -= creature_ptr->csp;
        creature_ptr->csp = 0;
        take_hit(creature_ptr, DAMAGE_USELIFE, actual_racial_cost, _("過度の集中", "concentrating too hard"), -1);
    } else
        creature_ptr->csp -= actual_racial_cost;

    creature_ptr->redraw |= PR_HP | PR_MANA;
    creature_ptr->window |= PW_PLAYER | PW_SPELL;
}
