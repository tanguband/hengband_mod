﻿#pragma once

#include "monster/monster-race.h"

typedef bool(*monsterrace_hook_type)(MONRACE_IDX r_idx);

extern MONSTER_IDX hack_m_idx;
extern MONSTER_IDX hack_m_idx_ii;

/* Maximum "Nazguls" number */
#define MAX_NAZGUL_NUM 5

#define MTIMED_CSLEEP   0 /* Monster is sleeping */
#define MTIMED_FAST     1 /* Monster is temporarily fast */
#define MTIMED_SLOW     2 /* Monster is temporarily slow */
#define MTIMED_STUNNED  3 /* Monster is stunned */
#define MTIMED_CONFUSED 4 /* Monster is confused */
#define MTIMED_MONFEAR  5 /* Monster is afraid */
#define MTIMED_INVULNER 6 /* Monster is temporarily invulnerable */

#define MAX_MTIMED      7

#define MON_CSLEEP(M_PTR)   ((M_PTR)->mtimed[MTIMED_CSLEEP])
#define MON_FAST(M_PTR)     ((M_PTR)->mtimed[MTIMED_FAST])
#define MON_SLOW(M_PTR)     ((M_PTR)->mtimed[MTIMED_SLOW])
#define MON_STUNNED(M_PTR)  ((M_PTR)->mtimed[MTIMED_STUNNED])
#define MON_CONFUSED(M_PTR) ((M_PTR)->mtimed[MTIMED_CONFUSED])
#define MON_MONFEAR(M_PTR)  ((M_PTR)->mtimed[MTIMED_MONFEAR])
#define MON_INVULNER(M_PTR) ((M_PTR)->mtimed[MTIMED_INVULNER])

/*
 * Monster information, for a specific monster.
 * Note: fy, fx constrain dungeon size to 256x256
 * The "hold_o_idx" field points to the first object of a stack
 * of objects (if any) being carried by the monster (see above).
 */

typedef struct floor_type floor_type;
typedef struct
{
	MONRACE_IDX r_idx;		/* Monster race index 0 = dead. */
	MONRACE_IDX ap_r_idx;	/* Monster race appearance index */
	floor_type *current_floor_ptr;

	/* Sub-alignment flags for neutral monsters */
	#define SUB_ALIGN_NEUTRAL 0x0000
	#define SUB_ALIGN_EVIL    0x0001
	#define SUB_ALIGN_GOOD    0x0002
	BIT_FLAGS8 sub_align;		/* Sub-alignment for a neutral monster */

	POSITION fy;		/* Y location on map */
	POSITION fx;		/* X location on map */

	HIT_POINT hp;		/* Current Hit points */
	HIT_POINT maxhp;		/* Max Hit points */
	HIT_POINT max_maxhp;		/* Max Max Hit points */
	HIT_POINT dealt_damage;		/* Sum of damages dealt by player */

	TIME_EFFECT mtimed[MAX_MTIMED];	/* Timed status counter */

	SPEED mspeed;	        /* Monster "speed" */
	ACTION_ENERGY energy_need;	/* Monster "energy" */

	POSITION cdis;		/* Current dis from player */

	BIT_FLAGS8 mflag;	/* Extra monster flags */
#define MFLAG_VIEW      0x01    /* Monster is in line of sight */
#define MFLAG_LOS       0x02    /* Monster is marked for project_all_los(caster_ptr, ) */
#define MFLAG_XXX2      0x04    /* (unused) */
#define MFLAG_ETF       0x08    /* Monster is entering the field. */
#define MFLAG_BORN      0x10    /* Monster is still being born */
#define MFLAG_NICE      0x20    /* Monster is still being nice */

	BIT_FLAGS8 mflag2;	/* Extra monster flags */
#define MFLAG2_KAGE      0x01    /* Monster is kage */
#define MFLAG2_NOPET     0x02    /* Cannot make monster pet */
#define MFLAG2_NOGENO    0x04    /* Cannot genocide */
#define MFLAG2_CHAMELEON 0x08    /* Monster is chameleon */
#define MFLAG2_NOFLOW    0x10    /* Monster is in no_flow_by_smell mode */
#define MFLAG2_SHOW      0x20    /* Monster is recently memorized */
#define MFLAG2_MARK      0x40    /* Monster is currently memorized */

	bool ml;		/* Monster is "visible" */

	OBJECT_IDX hold_o_idx;	/* Object being held (if any) */

	POSITION target_y;		/* Can attack !los player */
	POSITION target_x;		/* Can attack !los player */

	STR_OFFSET nickname;		/* Monster's Nickname */

	EXP exp;

	/* TODO: クローン、ペット、有効化は意義が異なるので別変数に切り離すこと。save/loadのバージョン更新が面倒そうだけど */
	BIT_FLAGS smart; /*!< Field for "smart_learn" - Some bit-flags for the "smart" field */
#define SM_RES_ACID             0x00000001 /*!< モンスターの学習フラグ: プレイヤーに酸耐性あり */
#define SM_RES_ELEC             0x00000002 /*!< モンスターの学習フラグ: プレイヤーに電撃耐性あり */
#define SM_RES_FIRE             0x00000004 /*!< モンスターの学習フラグ: プレイヤーに火炎耐性あり */
#define SM_RES_COLD             0x00000008 /*!< モンスターの学習フラグ: プレイヤーに冷気耐性あり */
#define SM_RES_POIS             0x00000010 /*!< モンスターの学習フラグ: プレイヤーに毒耐性あり */
#define SM_RES_NETH             0x00000020 /*!< モンスターの学習フラグ: プレイヤーに地獄耐性あり */
#define SM_RES_LITE             0x00000040 /*!< モンスターの学習フラグ: プレイヤーに閃光耐性あり */
#define SM_RES_DARK             0x00000080 /*!< モンスターの学習フラグ: プレイヤーに暗黒耐性あり */
#define SM_RES_FEAR             0x00000100 /*!< モンスターの学習フラグ: プレイヤーに恐怖耐性あり */
#define SM_RES_CONF             0x00000200 /*!< モンスターの学習フラグ: プレイヤーに混乱耐性あり */
#define SM_RES_CHAOS            0x00000400 /*!< モンスターの学習フラグ: プレイヤーにカオス耐性あり */
#define SM_RES_DISEN            0x00000800 /*!< モンスターの学習フラグ: プレイヤーに劣化耐性あり */
#define SM_RES_BLIND            0x00001000 /*!< モンスターの学習フラグ: プレイヤーに盲目耐性あり */
#define SM_RES_NEXUS            0x00002000 /*!< モンスターの学習フラグ: プレイヤーに因果混乱耐性あり */
#define SM_RES_SOUND            0x00004000 /*!< モンスターの学習フラグ: プレイヤーに轟音耐性あり */
#define SM_RES_SHARD            0x00008000 /*!< モンスターの学習フラグ: プレイヤーに破片耐性あり */
#define SM_OPP_ACID             0x00010000 /*!< モンスターの学習フラグ: プレイヤーに二重酸耐性あり */
#define SM_OPP_ELEC             0x00020000 /*!< モンスターの学習フラグ: プレイヤーに二重電撃耐性あり */
#define SM_OPP_FIRE             0x00040000 /*!< モンスターの学習フラグ: プレイヤーに二重火炎耐性あり */
#define SM_OPP_COLD             0x00080000 /*!< モンスターの学習フラグ: プレイヤーに二重冷気耐性あり */
#define SM_OPP_POIS             0x00100000 /*!< モンスターの学習フラグ: プレイヤーに二重毒耐性あり */
#define SM_OPP_XXX1             0x00200000 /*!< 未使用 / (unused) */
#define SM_CLONED               0x00400000 /*!< クローンである / Cloned */
#define SM_PET                  0x00800000 /*!< ペットである / Pet */
#define SM_IMM_ACID             0x01000000 /*!< モンスターの学習フラグ: プレイヤーに酸免疫あり */
#define SM_IMM_ELEC             0x02000000 /*!< モンスターの学習フラグ: プレイヤーに電撃免疫あり */
#define SM_IMM_FIRE             0x04000000 /*!< モンスターの学習フラグ: プレイヤーに火炎免疫あり */
#define SM_IMM_COLD             0x08000000 /*!< モンスターの学習フラグ: プレイヤーに冷気免疫あり */
#define SM_FRIENDLY             0x10000000 /*!< 友好的である / Friendly */
#define SM_IMM_REFLECT          0x20000000 /*!< モンスターの学習フラグ: プレイヤーに反射あり */
#define SM_IMM_FREE             0x40000000 /*!< モンスターの学習フラグ: プレイヤーに麻痺耐性あり */
#define SM_IMM_MANA             0x80000000 /*!< モンスターの学習フラグ: プレイヤーにMPがない */

	MONSTER_IDX parent_m_idx;
} monster_type;

/*
 * Bit flags for the place_monster_???() (etc)
 */
#define PM_ALLOW_SLEEP    0x00000001    /*!< モンスター生成フラグ: 眠っている状態で生成されても良い */
#define PM_ALLOW_GROUP    0x00000002    /*!< モンスター生成フラグ: 集団生成されても良い */
#define PM_FORCE_FRIENDLY 0x00000004    /*!< モンスター生成フラグ: 必ず友好的に生成される */
#define PM_FORCE_PET      0x00000008    /*!< モンスター生成フラグ: 必ずペットとして生成される */
#define PM_NO_KAGE        0x00000010    /*!< モンスター生成フラグ: 必ずあやしい影としては生成されない */
#define PM_NO_PET         0x00000020    /*!< モンスター生成フラグ: 必ずペットとして生成されない */
#define PM_ALLOW_UNIQUE   0x00000040    /*!< モンスター生成フラグ: ユニークの選択生成を許可する */
#define PM_IGNORE_TERRAIN 0x00000080    /*!< モンスター生成フラグ: 侵入可能地形を考慮せずに生成する */
#define PM_HASTE          0x00000100    /*!< モンスター生成フラグ: 加速状態で生成する */
#define PM_KAGE           0x00000200    /*!< モンスター生成フラグ: 必ずあやしい影として生成する */
#define PM_MULTIPLY       0x00000400    /*!< モンスター生成フラグ: 増殖処理時として生成する */
#define PM_JURAL          0x00000800    /*!< モンスター生成フラグ: ジュラル星人として誤認生成する */

extern bool place_monster_aux(player_type *player_ptr, MONSTER_IDX who, POSITION y, POSITION x, MONRACE_IDX r_idx, BIT_FLAGS mode);
extern bool place_monster(player_type *player_ptr, POSITION y, POSITION x, BIT_FLAGS mode);
extern bool alloc_horde(player_type *player_ptr, POSITION y, POSITION x);
extern bool alloc_guardian(player_type *player_ptr, bool def_val);
extern bool alloc_monster(player_type *player_ptr, POSITION dis, BIT_FLAGS mode);

extern void monster_desc(player_type *player_ptr, char *desc, monster_type *m_ptr, BIT_FLAGS mode);
/* Bit flags for monster_desc() */
#define MD_OBJECTIVE      0x00000001 /* Objective (or Reflexive) */
#define MD_POSSESSIVE     0x00000002 /* Possessive (or Reflexive) */
#define MD_INDEF_HIDDEN   0x00000004 /* Use indefinites for hidden monsters ("something") */
#define MD_INDEF_VISIBLE  0x00000008 /* Use indefinites for visible monsters ("a kobold") */
#define MD_PRON_HIDDEN    0x00000010 /* Pronominalize hidden monsters */
#define MD_PRON_VISIBLE   0x00000020 /* Pronominalize visible monsters */
#define MD_ASSUME_HIDDEN  0x00000040 /* Assume the monster is hidden */
#define MD_ASSUME_VISIBLE 0x00000080 /* Assume the monster is visible */
#define MD_TRUE_NAME      0x00000100 /* Chameleon's true name */
#define MD_IGNORE_HALLU   0x00000200 /* Ignore hallucination, and penetrate shape change */

#define MD_WRONGDOER_NAME (MD_IGNORE_HALLU | MD_ASSUME_VISIBLE | MD_INDEF_VISIBLE) /* 加害明記向け */

extern void monster_name(player_type *player_ptr, MONSTER_IDX m_idx, char* m_name);

extern void roff_top(MONRACE_IDX r_idx);
extern void screen_roff(player_type *player_ptr, MONRACE_IDX r_idx, BIT_FLAGS mode);
extern void display_roff(player_type *player_ptr);
extern void output_monster_spoiler(player_type *player_ptr, MONRACE_IDX r_idx, void(*roff_func)(TERM_COLOR attr, concptr str));
extern concptr extract_note_dies(MONRACE_IDX r_idx);
extern void monster_death(player_type *player_ptr, MONSTER_IDX m_idx, bool drop_item);
extern monsterrace_hook_type get_monster_hook(player_type *player_ptr);
extern monsterrace_hook_type get_monster_hook2(player_type *player_ptr, POSITION y, POSITION x);
extern void set_friendly(monster_type *m_ptr);
extern void set_pet(player_type *player_ptr, monster_type *m_ptr);
extern void set_hostile(player_type *player_ptr, monster_type *m_ptr);
extern void anger_monster(player_type *player_ptr, monster_type *m_ptr);

/*
 * Bit flags for the *_can_enter() and monster_can_cross_terrain()
 */
#define CEM_RIDING              0x0001
#define CEM_P_CAN_ENTER_PATTERN 0x0002
extern bool monster_can_cross_terrain(player_type *player_ptr, FEAT_IDX feat, monster_race *r_ptr, BIT_FLAGS16 mode);
extern bool monster_can_enter(player_type *player_ptr, POSITION y, POSITION x, monster_race *r_ptr, BIT_FLAGS16 mode);

extern bool are_enemies(player_type *player_ptr, monster_type *m_ptr1, monster_type *m_ptr2);
extern bool monster_has_hostile_align(player_type *player_ptr, monster_type *m_ptr, int pa_good, int pa_evil, monster_race *r_ptr);
extern void dice_to_string(int base_damage, int dice_num, int dice_side, int dice_mult, int dice_div, char* msg);
extern concptr look_mon_desc(monster_type *m_ptr, BIT_FLAGS mode);
extern int get_monster_crowd_number(player_type *player_ptr, MONSTER_IDX m_idx);
extern void message_pain(player_type *player_ptr, MONSTER_IDX m_idx, HIT_POINT dam);

/* monster2.c */
extern void set_target(monster_type *m_ptr, POSITION y, POSITION x);
extern void reset_target(monster_type *m_ptr);
extern monster_race *real_r_ptr(monster_type *m_ptr);
extern MONRACE_IDX real_r_idx(monster_type *m_ptr);
extern void delete_monster_idx(player_type *player_ptr, MONSTER_IDX i);
extern void compact_monsters(player_type *player_ptr, int size);
extern void wipe_monsters_list(player_type *player_ptr);
extern MONSTER_IDX m_pop(player_type *player_ptr);
extern errr get_mon_num_prep(player_type *player_ptr, monsterrace_hook_type monster_hook, monsterrace_hook_type monster_hook2);

#define GMN_ARENA 0x00000001 //!< 賭け闘技場向け生成 
extern MONRACE_IDX get_mon_num(player_type *player_ptr, DEPTH level, BIT_FLAGS option);

extern int lore_do_probe(player_type *player_ptr, MONRACE_IDX r_idx);
extern void lore_treasure(player_type *player_ptr, MONSTER_IDX m_idx, ITEM_NUMBER num_item, ITEM_NUMBER num_gold);
extern void update_monster(player_type *subject_ptr, MONSTER_IDX m_idx, bool full);
extern void update_monsters(player_type *player_ptr, bool full);
extern bool multiply_monster(player_type *player_ptr, MONSTER_IDX m_idx, bool clone, BIT_FLAGS mode);
extern bool summon_specific(player_type *player_ptr, MONSTER_IDX who, POSITION y1, POSITION x1, DEPTH lev, int type, BIT_FLAGS mode);
extern bool summon_named_creature(player_type *player_ptr, MONSTER_IDX who, POSITION oy, POSITION ox, MONRACE_IDX r_idx, BIT_FLAGS mode);

/*
 * Some things which induce learning
 */
#define DRS_ACID         1
#define DRS_ELEC         2
#define DRS_FIRE         3
#define DRS_COLD         4
#define DRS_POIS         5
#define DRS_NETH         6
#define DRS_LITE         7
#define DRS_DARK         8
#define DRS_FEAR         9
#define DRS_CONF        10
#define DRS_CHAOS       11
#define DRS_DISEN       12
#define DRS_BLIND       13
#define DRS_NEXUS       14
#define DRS_SOUND       15
#define DRS_SHARD       16
#define DRS_FREE        30
#define DRS_MANA        31
#define DRS_REFLECT     32
extern void update_smart_learn(player_type *player_ptr, MONSTER_IDX m_idx, int what);

extern void choose_new_monster(player_type *player_ptr, MONSTER_IDX m_idx, bool born, MONRACE_IDX r_idx);
extern SPEED get_mspeed(player_type *player_ptr, monster_race *r_ptr);
extern void monster_drop_carried_objects(player_type *player_ptr, monster_type *m_ptr);
extern bool is_original_ap_and_seen(player_type *player_ptr, monster_type *m_ptr);

#define is_friendly(A) \
	 (bool)(((A)->smart & SM_FRIENDLY) ? TRUE : FALSE)

#define is_pet(A) \
	 (bool)(((A)->smart & SM_PET) ? TRUE : FALSE)

#define is_hostile(A) \
	 (bool)((is_friendly(A) || is_pet(A)) ? FALSE : TRUE)

/* Hack -- Determine monster race appearance index is same as race index */
#define is_original_ap(A) \
	 (bool)(((A)->ap_r_idx == (A)->r_idx) ? TRUE : FALSE)
