﻿#include "mutation/mutation-util.h"
#include "mutation/mutation-flag-types.h"

glm_type *initialize_glm_type(glm_type *gm_ptr, MUTATION_IDX choose_mut)
{
    gm_ptr->muta_which = MUTA::MAX;
    gm_ptr->muta_desc = "";
    gm_ptr->muta_chosen = FALSE;
    gm_ptr->choose_mut = choose_mut;
    return gm_ptr;
}
