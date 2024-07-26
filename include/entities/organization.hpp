#pragma once

#include "entities/entity.hpp"

class Organization : public Entity {
public:
    UpdateResult process_order(Order & order, GameState * state) override;
    UpdateResult update(GameState * game_state, float delta_time) override;
private:
    int population;
};