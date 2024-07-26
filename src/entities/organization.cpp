#include "entities/organization.hpp"


UpdateResult Organization::process_order(Order & order, GameState * state) {
    (void)order;
    (void)state;
    return {};
}


UpdateResult Organization::update(GameState * game_state, float delta_time) {
    (void)game_state;
    (void)delta_time;

    return UpdateResult{};
}