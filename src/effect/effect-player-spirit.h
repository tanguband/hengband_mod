﻿#pragma once

typedef struct effect_player_type effect_player_type;
typedef struct player_type player_type;
void effect_player_drain_mana(player_type *target_ptr, effect_player_type *ep_ptr);
void effect_player_mind_blast(player_type *target_ptr, effect_player_type *ep_ptr);
void effect_player_brain_smash(player_type *target_ptr, effect_player_type *ep_ptr);
