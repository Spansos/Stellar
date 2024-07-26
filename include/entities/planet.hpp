#pragma once

#include <array>
#include "entities/entity.hpp"

class Planet : public Entity {
public:
    Planet(sf::Vector2f position, EntityID id, EntityID owner);
    UpdateResult process_order(Order & order, GameState * state) override;
    void update_state(StateChange change) override;
    PropertyValue get_property(PropertyID property) override;
    UpdateResult update(GameState * game_state, float delta_time) override;
    enum Properties {POSITION, POPULATION, ORGANIZATION_N};
private:
    sf::Vector2f position;
    std::uint64_t population;
    std::array<EntityID, 50> organizations;
};