﻿#include "mind/racial-balrog.h"
#include "io/targeting.h"
#include "spell-kind/spells-launcher.h"
#include "spell/spells-type.h"

bool demonic_breath(player_type *creature_ptr)
{
    DIRECTION dir;
    int type = (one_in_(2) ? GF_NETHER : GF_FIRE);
    if (!get_aim_dir(creature_ptr, &dir))
        return FALSE;
    stop_mouth(creature_ptr);
    msg_format(_("あなたは%sのブレスを吐いた。", "You breathe %s."), ((type == GF_NETHER) ? _("地獄", "nether") : _("火炎", "fire")));
    fire_breath(creature_ptr, type, dir, creature_ptr->lev * 3, (creature_ptr->lev / 15) + 1);
    return TRUE;
}