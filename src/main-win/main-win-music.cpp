﻿/*!
 * @file main-win-music.cpp
 * @brief Windows版固有実装(BGM)
 */

#include "main-win/main-win-music.h"
#include "main-win/main-win-define.h"
#include "main-win/main-win-file-utils.h"
#include "main-win/main-win-mci.h"
#include "main-win/main-win-mmsystem.h"
#include "main-win/main-win-tokenizer.h"
#include "term/z-term.h"
#include "util/angband-files.h"

concptr music_file[MUSIC_BASIC_MAX][SAMPLE_MUSIC_MAX];
// TODO マジックナンバー除去
concptr dungeon_music_file[1000][SAMPLE_MUSIC_MAX];
concptr town_music_file[1000][SAMPLE_MUSIC_MAX];
concptr quest_music_file[1000][SAMPLE_MUSIC_MAX];

static int current_music_type = TERM_XTRA_MUSIC_MUTE;
static int current_music_id = 0;
static char current_music_path[MAIN_WIN_MAX_PATH];

/*
 * Directory name
 */
concptr ANGBAND_DIR_XTRA_MUSIC;

namespace main_win_music {
void load_music_prefs(DUNGEON_IDX max_d_idx, QUEST_IDX max_q_idx)
{
    char tmp[MAIN_WIN_MAX_PATH];
    char ini_path[MAIN_WIN_MAX_PATH];
    char wav_path[MAIN_WIN_MAX_PATH];
    char *zz[SAMPLE_MUSIC_MAX];
    // TODO マジックナンバー除去
    char key[80];

    path_build(ini_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, "music_debug.cfg");
    if (GetPrivateProfileString("Device", "type", "", mci_device_type, _countof(mci_device_type), ini_path) == 0) {
        path_build(ini_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, "music.cfg");
        GetPrivateProfileString("Device", "type", "", mci_device_type, _countof(mci_device_type), ini_path);
    }

    for (int i = 0; i < MUSIC_BASIC_MAX; i++) {
        GetPrivateProfileString("Basic", angband_music_basic_name[i], "", tmp, MAIN_WIN_MAX_PATH, ini_path);
        int num = tokenize_whitespace(tmp, SAMPLE_MUSIC_MAX, zz);
        for (int j = 0; j < num; j++) {
            path_build(wav_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, zz[j]);
            if (check_file(wav_path))
                music_file[i][j] = string_make(zz[j]);
        }
    }

    for (int i = 0; i < max_d_idx; i++) {
        sprintf(key, "dungeon%03d", i);
        GetPrivateProfileString("Dungeon", key, "", tmp, MAIN_WIN_MAX_PATH, ini_path);
        int num = tokenize_whitespace(tmp, SAMPLE_MUSIC_MAX, zz);
        for (int j = 0; j < num; j++) {
            path_build(wav_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, zz[j]);
            if (check_file(wav_path))
                dungeon_music_file[i][j] = string_make(zz[j]);
        }
    }

    for (int i = 0; i < max_q_idx; i++) {
        sprintf(key, "quest%03d", i);
        GetPrivateProfileString("Quest", key, "", tmp, MAIN_WIN_MAX_PATH, ini_path);
        int num = tokenize_whitespace(tmp, SAMPLE_MUSIC_MAX, zz);
        for (int j = 0; j < num; j++) {
            path_build(wav_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, zz[j]);
            if (check_file(wav_path))
                quest_music_file[i][j] = string_make(zz[j]);
        }
    }

    for (int i = 0; i < 1000; i++) /*!< @todo 町最大数指定 */
    {
        sprintf(key, "town%03d", i);
        GetPrivateProfileString("Town", key, "", tmp, MAIN_WIN_MAX_PATH, ini_path);
        int num = tokenize_whitespace(tmp, SAMPLE_MUSIC_MAX, zz);
        for (int j = 0; j < num; j++) {
            path_build(wav_path, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, zz[j]);
            if (check_file(wav_path))
                town_music_file[i][j] = string_make(zz[j]);
        }
    }
}

/*
 * Stop a music
 */
void stop_music(void)
{
    mciSendCommand(mop.wDeviceID, MCI_STOP, 0, 0);
    mciSendCommand(mop.wDeviceID, MCI_CLOSE, 0, 0);
    current_music_type = TERM_XTRA_MUSIC_MUTE;
    current_music_id = 0;
    strcpy(current_music_path, "\0");
}

/*
 * Play a music
 */
errr play_music(int type, int val)
{
    int i = 0;
    char buf[MAIN_WIN_MAX_PATH];

    if (type == TERM_XTRA_MUSIC_MUTE)
        stop_music();

    if (type == TERM_XTRA_MUSIC_BASIC && ((val < 0) || (val >= MUSIC_BASIC_MAX)))
        return 1;
    else if (val < 0 || val >= 1000)
        return (1); /*!< TODO */

    switch (type) {
    case TERM_XTRA_MUSIC_BASIC:
        for (i = 0; i < SAMPLE_MUSIC_MAX; i++)
            if (!music_file[val][i])
                break;
        break;
    case TERM_XTRA_MUSIC_DUNGEON:
        for (i = 0; i < SAMPLE_MUSIC_MAX; i++)
            if (!dungeon_music_file[val][i])
                break;
        break;
    case TERM_XTRA_MUSIC_QUEST:
        for (i = 0; i < SAMPLE_MUSIC_MAX; i++)
            if (!quest_music_file[val][i])
                break;
        break;
    case TERM_XTRA_MUSIC_TOWN:
        for (i = 0; i < SAMPLE_MUSIC_MAX; i++)
            if (!town_music_file[val][i])
                break;
        break;
    }

    if (i == 0) {
        return 1;
    }

    switch (type) {
    case TERM_XTRA_MUSIC_BASIC:
        path_build(buf, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, music_file[val][Rand_external(i)]);
        break;
    case TERM_XTRA_MUSIC_DUNGEON:
        path_build(buf, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, dungeon_music_file[val][Rand_external(i)]);
        break;
    case TERM_XTRA_MUSIC_QUEST:
        path_build(buf, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, quest_music_file[val][Rand_external(i)]);
        break;
    case TERM_XTRA_MUSIC_TOWN:
        path_build(buf, MAIN_WIN_MAX_PATH, ANGBAND_DIR_XTRA_MUSIC, town_music_file[val][Rand_external(i)]);
        break;
    }

    if (current_music_type == type && current_music_id == val)
        return 0;

    if (current_music_type != TERM_XTRA_MUSIC_MUTE && type != TERM_XTRA_MUSIC_MUTE)
        if (0 == strcmp(current_music_path, buf))
            return 0;

    current_music_type = type;
    current_music_id = val;
    strcpy(current_music_path, buf);

    mop.lpstrDeviceType = mci_device_type;
    mop.lpstrElementName = buf;
    mciSendCommand(mop.wDeviceID, MCI_STOP, 0, 0);
    mciSendCommand(mop.wDeviceID, MCI_CLOSE, 0, 0);
    mciSendCommand(mop.wDeviceID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mop);
    mciSendCommand(mop.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
    mciSendCommand(mop.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&mop);
    return 0;
}
}
