﻿#pragma once

enum class SaveDataItemFlagType {
	PVAL = 0x00000001,
	DISCOUNT = 0x00000002,
	NUMBER = 0x00000004,
	NAME1 = 0x00000008,
	NAME2 = 0x00000010,
	TIMEOUT = 0x00000020,
	TO_H = 0x00000040,
	TO_D = 0x00000080,
	TO_A = 0x00000100,
	AC = 0x00000200,
	DD = 0x00000400,
	DS = 0x00000800,
	IDENT = 0x00001000,
	MARKED = 0x00002000,
	SMITH = 0x00004000,
	XXX1 = 0x00008000, //<! 未使用
	XXX2 = 0x00010000, //<! 未使用
	XXX3 = 0x00020000, //<! 未使用
	CURSE_FLAGS = 0x00040000,
	HELD_M_IDX = 0x00080000,
	XTRA1 = 0x00100000,
	ACTIVATION_ID = 0x00200000,
	XTRA3 = 0x00400000,
	XTRA4 = 0x00800000,
	XTRA5 = 0x01000000,
	FEELING = 0x02000000,
	INSCRIPTION = 0x04000000,
	ART_NAME = 0x08000000,
	ART_FLAGS = 0x10000000,
	STACK_IDX = 0x20000000,
};
