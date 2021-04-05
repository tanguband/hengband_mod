﻿#pragma once

#include "system/angband.h"

enum class MUTA;

typedef struct glm_type {
    MUTA muta_which;
    concptr muta_desc;
    bool muta_chosen;
    MUTATION_IDX choose_mut;
} glm_type;

glm_type *initialize_glm_type(glm_type *gm_ptr, MUTATION_IDX choose_mut);
