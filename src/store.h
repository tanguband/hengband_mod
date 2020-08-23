﻿
/*
 * Store types
 */
#define STORE_GENERAL   0 /*!< 店舗の種類: 雑貨屋 */
#define STORE_ARMOURY   1 /*!< 店舗の種類: 防具屋 */
#define STORE_WEAPON    2 /*!< 店舗の種類: 武器屋 */
#define STORE_TEMPLE    3 /*!< 店舗の種類: 寺院 */
#define STORE_ALCHEMIST 4 /*!< 店舗の種類: 錬金術の店 */
#define STORE_MAGIC     5 /*!< 店舗の種類: 魔道具屋 */
#define STORE_BLACK     6 /*!< 店舗の種類: ブラック・マーケット */
#define STORE_HOME      7 /*!< 店舗の種類: 我が家 */
#define STORE_BOOK      8 /*!< 店舗の種類: 書店 */
#define STORE_MUSEUM    9 /*!< 店舗の種類: 博物館 */
#define MAX_STORES      10 /*!< store.c用の店舗の種類最大数 / Total number of stores (see "store.c", etc) */

#define MAX_OWNERS      32 /*!< 各店舗毎の店主定義最大数 / Total number of owners per store (see "store.c", etc) */

 /*
  * Store constants
  */
#define STORE_INVEN_MAX 24              /* Max number of discrete objs in inven */
#define STORE_CHOICES   48              /* Number of items to choose stock from */
#define STORE_OBJ_LEVEL 5               /* Magic Level for normal stores */
#define STORE_TURNOVER  9               /* Normal shop turnover, per day */
#define STORE_MIN_KEEP  6               /* Min slots to "always" keep full */
#define STORE_MAX_KEEP  18              /* Max slots to "always" keep full */
#define STORE_SHUFFLE   21              /* 1/Chance (per day) of an owner changing */
#define STORE_TICKS     1000            /* Number of ticks between turnovers */

/* store.c */
extern bool combine_and_reorder_home(int store_num);
extern void do_cmd_store(void);
extern void store_shuffle(int which);
extern void store_maint(int town_num, int store_num);
extern void store_init(int town_num, int store_num);
extern void move_to_black_market(object_type * o_ptr);

extern const owner_type owners[MAX_STORES][MAX_OWNERS];

extern byte store_table[MAX_STORES][STORE_CHOICES][2];



