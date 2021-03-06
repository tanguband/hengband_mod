﻿#pragma once
#include "player-status/player-basic-statistics.h"

class PlayerConstitution : public PlayerBasicStatistics {
public:
    using PlayerBasicStatistics::PlayerBasicStatistics;
    PlayerConstitution() = delete;

protected:
    void set_locals() override;
    s16b race_value() override;
    s16b time_effect_value() override;
    s16b battleform_value() override;
    s16b mutation_value() override;
};
