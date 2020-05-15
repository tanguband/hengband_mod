﻿#include "system/angband.h"
#include "effect/effect-player-util.h"
#include "effect/effect-player-curse.h"
#include "player/player-damage.h"
#include "world/world.h"
#include "spell/monster-spell.h"
#include "object/object-curse.h"
#include "player/player-effects.h"

void effect_player_curse_1(player_type *target_ptr,
                           effect_player_type *ep_ptr) {
  if ((randint0(100 + ep_ptr->rlev / 2) < target_ptr->skill_sav) &&
      !CHECK_MULTISHADOW(target_ptr)) {
    msg_print(_("しかし効力を跳ね返した！", "You resist the effects!"));
    learn_spell(target_ptr, ep_ptr->monspell);
  } else {
    if (!CHECK_MULTISHADOW(target_ptr))
      curse_equipment(target_ptr, 15, 0);
    ep_ptr->get_damage = take_hit(target_ptr, DAMAGE_ATTACK, ep_ptr->dam,
                                  ep_ptr->killer, ep_ptr->monspell);
  }
}

void effect_player_curse_2(player_type *target_ptr,
                           effect_player_type *ep_ptr) {
  if ((randint0(100 + ep_ptr->rlev / 2) < target_ptr->skill_sav) &&
      !CHECK_MULTISHADOW(target_ptr)) {
    msg_print(_("しかし効力を跳ね返した！", "You resist the effects!"));
    learn_spell(target_ptr, ep_ptr->monspell);
  } else {
    if (!CHECK_MULTISHADOW(target_ptr))
      curse_equipment(target_ptr, 25, MIN(ep_ptr->rlev / 2 - 15, 5));
    ep_ptr->get_damage = take_hit(target_ptr, DAMAGE_ATTACK, ep_ptr->dam,
                                  ep_ptr->killer, ep_ptr->monspell);
  }
}

void effect_player_curse_3(player_type *target_ptr,
                           effect_player_type *ep_ptr) {
  if ((randint0(100 + ep_ptr->rlev / 2) < target_ptr->skill_sav) &&
      !CHECK_MULTISHADOW(target_ptr)) {
    msg_print(_("しかし効力を跳ね返した！", "You resist the effects!"));
    learn_spell(target_ptr, ep_ptr->monspell);
  } else {
    if (!CHECK_MULTISHADOW(target_ptr))
      curse_equipment(target_ptr, 33, MIN(ep_ptr->rlev / 2 - 15, 15));
    ep_ptr->get_damage = take_hit(target_ptr, DAMAGE_ATTACK, ep_ptr->dam,
                                  ep_ptr->killer, ep_ptr->monspell);
  }
}

void effect_player_curse_4(player_type *target_ptr,
                           effect_player_type *ep_ptr) {
  if ((randint0(100 + ep_ptr->rlev / 2) < target_ptr->skill_sav) &&
      !(ep_ptr->m_ptr->r_idx == MON_KENSHIROU) &&
      !CHECK_MULTISHADOW(target_ptr)) {
    msg_print(_("しかし秘孔を跳ね返した！", "You resist the effects!"));
    learn_spell(target_ptr, ep_ptr->monspell);
  } else {
    ep_ptr->get_damage = take_hit(target_ptr, DAMAGE_ATTACK, ep_ptr->dam,
                                  ep_ptr->killer, ep_ptr->monspell);
    if (!CHECK_MULTISHADOW(target_ptr))
      (void)set_cut(target_ptr, target_ptr->cut + damroll(10, 10));
  }
}
