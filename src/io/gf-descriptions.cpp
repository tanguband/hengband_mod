﻿/*!
 * @brief Index of spell type names
 */

#include "gf-descriptions.h"
#include "spell/spell-types.h"

const named_num gf_desc[MAX_NAMED_NUM] =
{
	{"GF_ELEC", 				GF_ELEC				},
	{"GF_POIS", 				GF_POIS				},
	{"GF_ACID", 				GF_ACID				},
	{"GF_COLD", 				GF_COLD				},
	{"GF_FIRE",		 			GF_FIRE				},
	{"GF_PSY_SPEAR",			GF_PSY_SPEAR		},
	{"GF_MISSILE",				GF_MISSILE			},
	{"GF_ARROW",				GF_ARROW			},
	{"GF_PLASMA",				GF_PLASMA			},
	{"GF_WATER",				GF_WATER			},
	{"GF_LITE",					GF_LITE				},
	{"GF_DARK",					GF_DARK				},
	{"GF_LITE_WEAK",			GF_LITE_WEAK		},
	{"GF_DARK_WEAK",			GF_DARK_WEAK		},
	{"GF_SHARDS",				GF_SHARDS			},
	{"GF_SOUND",				GF_SOUND			},
	{"GF_CONFUSION",			GF_CONFUSION		},
	{"GF_FORCE",				GF_FORCE			},
	{"GF_INERTIA",				GF_INERTIAL			},
	{"GF_MANA",					GF_MANA				},
	{"GF_METEOR",				GF_METEOR			},
	{"GF_ICE",					GF_ICE				},
	{"GF_CHAOS",				GF_CHAOS			},
	{"GF_NETHER",				GF_NETHER			},
	{"GF_DISENCHANT",			GF_DISENCHANT		},
	{"GF_NEXUS",				GF_NEXUS			},
	{"GF_TIME",					GF_TIME				},
	{"GF_GRAVITY",				GF_GRAVITY			},
	{"GF_KILL_WALL",			GF_KILL_WALL		},
	{"GF_KILL_DOOR",			GF_KILL_DOOR		},
	{"GF_KILL_TRAP",			GF_KILL_TRAP		},
	{"GF_MAKE_WALL",			GF_MAKE_WALL		},
	{"GF_MAKE_DOOR",			GF_MAKE_DOOR		},
	{"GF_MAKE_TRAP",			GF_MAKE_TRAP		},
	{"GF_MAKE_TREE",			GF_MAKE_TREE		},
	{"GF_OLD_CLONE",			GF_OLD_CLONE		},
	{"GF_OLD_POLY",				GF_OLD_POLY			},
	{"GF_OLD_HEAL",				GF_OLD_HEAL			},
	{"GF_OLD_SPEED",			GF_OLD_SPEED		},
	{"GF_OLD_SLOW",				GF_OLD_SLOW			},
	{"GF_OLD_CONF",				GF_OLD_CONF			},
	{"GF_OLD_SLEEP",			GF_OLD_SLEEP		},
	{"GF_HYPODYNAMIA",			GF_HYPODYNAMIA		},
	{"GF_AWAY_UNDEAD",			GF_AWAY_UNDEAD		},
	{"GF_AWAY_EVIL",			GF_AWAY_EVIL		},
	{"GF_AWAY_ALL",				GF_AWAY_ALL			},
	{"GF_TURN_UNDEAD",			GF_TURN_UNDEAD		},
	{"GF_TURN_EVIL",			GF_TURN_EVIL		},
	{"GF_TURN_ALL",				GF_TURN_ALL			},
	{"GF_DISP_UNDEAD",			GF_DISP_UNDEAD		},
	{"GF_DISP_EVIL",			GF_DISP_EVIL		},
	{"GF_DISP_ALL",				GF_DISP_ALL			},
	{"GF_DISP_DEMON",			GF_DISP_DEMON		},
	{"GF_DISP_LIVING",			GF_DISP_LIVING		},
	{"GF_ROCKET",				GF_ROCKET			},
	{"GF_NUKE",					GF_NUKE				},
    {"GF_MAKE_RUNE_PROTECTION",	GF_MAKE_RUNE_PROTECTION},
	{"GF_STASIS",				GF_STASIS			},
	{"GF_STONE_WALL",			GF_STONE_WALL		},
	{"GF_DEATH_RAY",			GF_DEATH_RAY		},
	{"GF_STUN",					GF_STUN				},
	{"GF_HOLY_FIRE",			GF_HOLY_FIRE		},
	{"GF_HELL_FIRE",			GF_HELL_FIRE		},
	{"GF_DISINTEGRATE",			GF_DISINTEGRATE		},
	{"GF_CHARM",				GF_CHARM			},
	{"GF_CONTROL_UNDEAD",		GF_CONTROL_UNDEAD	},
	{"GF_CONTROL_ANIMAL",		GF_CONTROL_ANIMAL	},
	{"GF_PSI",					GF_PSI				},
	{"GF_PSI_DRAIN",			GF_PSI_DRAIN		},
	{"GF_TELEKINESIS",			GF_TELEKINESIS		},
	{"GF_JAM_DOOR",				GF_JAM_DOOR			},
	{"GF_DOMINATION",			GF_DOMINATION		},
	{"GF_DISP_GOOD",			GF_DISP_GOOD		},
	{"GF_DRAIN_MANA",			GF_DRAIN_MANA		},
	{"GF_MIND_BLAST",			GF_MIND_BLAST		},
	{"GF_BRAIN_SMASH",			GF_BRAIN_SMASH		},
	{"GF_CAUSE_1",				GF_CAUSE_1			},
	{"GF_CAUSE_2",				GF_CAUSE_2			},
	{"GF_CAUSE_3",				GF_CAUSE_3			},
	{"GF_CAUSE_4",				GF_CAUSE_4			},
	{"GF_HAND_DOOM",			GF_HAND_DOOM		},
	{"GF_CAPTURE",				GF_CAPTURE			},
	{"GF_ANIM_DEAD",			GF_ANIM_DEAD		},
	{"GF_CHARM_LIVING",			GF_CHARM_LIVING		},
	{"GF_IDENTIFY",				GF_IDENTIFY			},
	{"GF_ATTACK",				GF_ATTACK			},
	{"GF_ENGETSU",				GF_ENGETSU			},
	{"GF_GENOCIDE",				GF_GENOCIDE			},
	{"GF_PHOTO",				GF_PHOTO			},
	{"GF_CONTROL_DEMON",		GF_CONTROL_DEMON	},
	{"GF_LAVA_FLOW",			GF_LAVA_FLOW		},
	{"GF_BLOOD_CURSE",			GF_BLOOD_CURSE		},
	{"GF_SEEKER",				GF_SEEKER			},
	{"GF_SUPER_RAY",			GF_SUPER_RAY		},
	{"GF_STAR_HEAL",			GF_STAR_HEAL		},
	{"GF_WATER_FLOW",			GF_WATER_FLOW		},
	{"GF_CRUSADE",				GF_CRUSADE			},
	{"GF_STASIS_EVIL",			GF_STASIS_EVIL		},
	{"GF_WOUNDS",				GF_WOUNDS			},
	{"GF_E_GENOCIDE",           GF_E_GENOCIDE       },
	{"GF_VOID",                 GF_VOID             },
	{"GF_ABYSS",                GF_ABYSS            },
};
