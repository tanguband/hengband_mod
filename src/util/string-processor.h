﻿#pragma once

#include "system/angband.h"

#include <string>
#include <string_view>
#include <vector>

#define MAX_MACRO_MOD 12
#define MAX_MACRO_TRIG 200 /*!< 登録を許すマクロ（トリガー）の最大数 */

extern const char hexsym[16];

/*
 * Automatically generated "variable" declarations
 */
extern int max_macrotrigger;
extern concptr macro_template;
extern concptr macro_modifier_chr;
extern concptr macro_modifier_name[MAX_MACRO_MOD];
extern concptr macro_trigger_name[MAX_MACRO_TRIG];
extern concptr macro_trigger_keycode[2][MAX_MACRO_TRIG];

void text_to_ascii(char *buf, concptr str);
void ascii_to_text(char *buf, concptr str);
size_t angband_strcpy(char *buf, concptr src, size_t bufsize);
size_t angband_strcat(char *buf, concptr src, size_t bufsize);
char *angband_strstr(concptr haystack, concptr needle);
char *angband_strchr(concptr ptr, char ch);
char *ltrim(char *p);
char *rtrim(char *p);
int strrncmp(const char *s1, const char *s2, int len);
std::string str_trim(std::string_view str);
std::string str_rtrim(std::string_view str);
std::string str_ltrim(std::string_view str);
std::vector<std::string> str_split(std::string_view str, char delim, bool trim = false, int num = 0);
std::string str_erase(std::string str, std::string_view erase_chars);
